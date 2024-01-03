#include "RenderBase.h"
#include "RenderWindow.h"
#include "TextureManager.h"
#include "ShaderCompilerManager.h"
#include <cassert>
#include <string>
#include <d3dcompiler.h>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

DirectX12CompileLib

using namespace Microsoft::WRL;

float RenderBase::sClearColor[4] = { 0.1f,0.25f,0.5f,0.0f };

void RenderBase::Init()
{
	// デバッグビルド時に処理を実行
	ProcessAtDebugBuild([]()
		{
			// デバッグレイヤーを有効化
			ComPtr<ID3D12Debug1> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
			{
				debugController->EnableDebugLayer();
				debugController->SetEnableGPUBasedValidation(false);
			}
		});

	DeviceInit(); // デバイスの初期化

	// デバッグビルド時に処理を実行
	ProcessAtDebugBuild([]()
		{
			ComPtr<ID3D12InfoQueue> infoQueue;
			if (SUCCEEDED(RenderBase::GetInstance()->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue))))
			{
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true); // 重大なエラーでブレーク
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);      // エラーでブレーク
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);    // 警告でブレーク
			}

			// 特定の警告メッセージをフィルタリング
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
			};

			// 特定の警告メッセージを無効化
			D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
			D3D12_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = _countof(denyIds);
			filter.DenyList.pIDList = denyIds;
			filter.DenyList.NumSeverities = _countof(severities);
			filter.DenyList.pSeverityList = severities;
			// 特定の警告メッセージをフィルタに追加
			infoQueue->PushStorageFilter(&filter);
		});

	mRenderWindow = RenderWindow::GetInstance().get();
	mViewport = std::make_unique<Viewport>();
	mScissorRectangle = std::make_unique<ScissorRectangle>();

	mFenceValue = 0;

	DescriptorHeapInit();   // ディスクリプタヒープの初期化
	CommandInit();          // コマンドの初期化
	SwapChainInit();        // スワップチェインの初期化
	FenceInit();            // フェンスの初期化
	DepthBufferInit();      // デプスバッファの初期化
	ShaderCompilerInit();   // シェーダーコンパイラの初期化
	GraphicsPipelineInit(); // グラフィックスパイプラインの初期化
	ComputePipelineInit();  // コンピュートパイプラインの初期化
}
void RenderBase::PreDraw()
{
	// バックバッファのインデックスを取得
	uint32_t bbIndex = mSwapChain->GetCurrentBackBufferIndex();
	// バックバッファをレンダーターゲットに遷移
	TransitionBufferState(
		mBackBuffers[bbIndex]->GetBufferResource(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	// ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mBackBuffers[bbIndex]->GetBufferResource()->rtvHandle.cpu;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDepthBuffer->GetBufferResource()->dsvHandle.cpu;
	mCommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// クリア
	mCommandList->ClearRenderTargetView(rtvHandle, sClearColor, 0, nullptr);
	mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポートの設定
	mViewport->SetViewport(
		{ 0,0 },
	{
		(float)mRenderWindow->GetWindowSize().x,
		(float)mRenderWindow->GetWindowSize().y
	});
	mViewport->DrawCommands();

	// シザー矩形の更新
	mScissorRectangle->SetRectAngle(
		{
			RectAngle(
				0, RenderWindow::GetInstance()->GetWindowSize().x,
				0, RenderWindow::GetInstance()->GetWindowSize().y)
		});
	mScissorRectangle->DrawCommands();
}
void RenderBase::PostDraw()
{
	HRESULT result;

	// バックバッファのインデックスを取得
	uint32_t bbIndex = mSwapChain->GetCurrentBackBufferIndex();
	// バックバッファから画面表示状態への遷移
	TransitionBufferState(
		mBackBuffers[bbIndex]->GetBufferResource(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	// コマンドリストを閉じる
	result = mCommandList->Close();
	assert(SUCCEEDED(result));
	// コマンドリストの実行
	ID3D12CommandList* mCommandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(1, mCommandLists);

	// 画面を表示
	result = mSwapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	// コマンドの実行を待機
	mCommandQueue->Signal(mFence.Get(), ++mFenceValue);
	if (mFence.Get()->GetCompletedValue() != mFenceValue)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// コマンドアロケータをリセット
	result = mCommandAllocator->Reset();
	assert(SUCCEEDED(result));
	// コマンドリストをリセット
	result = mCommandList.Get()->Reset(mCommandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void RenderBase::TransitionBufferState(
	BufferResource* bufferResource,
	const D3D12_RESOURCE_STATES currentState,
	const D3D12_RESOURCE_STATES targetState)
{
	if (bufferResource->bufferState != currentState)
	{
		return;
	}

	CD3DX12_RESOURCE_BARRIER barrier =
		CD3DX12_RESOURCE_BARRIER::Transition(
			bufferResource->buffer.Get(),
			currentState,
			targetState,
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
	bufferResource->bufferState = targetState;
	mCommandList->ResourceBarrier(1, &barrier);
}

// --- 初期化関連 ------------------------------------------------------------ //
void RenderBase::DeviceInit()
{
	HRESULT result;

	// DXGIファクトリの初期化
	result = CreateDXGIFactory(IID_PPV_ARGS(&mDxgiFactory));
	assert(SUCCEEDED(result));

	// 利用可能なアダプタのリストを取得
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// 高性能なGPUを優先してアダプタを列挙し、リストに追加
	for (uint32_t i = 0;
		mDxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter);
	}

	// 選択したアダプタを特定
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプタの詳細情報を取得
		adapters[i]->GetDesc3(&adapterDesc);
		// ソフトウェアアダプタでない場合
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// 選択したアダプタを保持
			tmpAdapter = adapters[i].Get();
			break;
		}
	}

	// デバイスの初期化
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++)
	{
		// デバイスの作成を試み、対応する機能レベルを取得
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(mDevice.GetAddressOf()));
		if (result == S_OK) {
			// デバイスが作成された場合、対応する機能レベルを保持
			featureLevel = levels[i];
			break;
		}
	}
}
void RenderBase::DescriptorHeapInit()
{
	DescriptorHeapSetting setting;
	// SRV用
	setting = DescriptorHeapSetting();
	setting.maxSize = 2048;
	setting.startIndex = 1;
	setting.heapType = DescriptorHeapSetting::CBV_SRV_UAV;
	DescriptorHeapManager::Create(setting, "SRV");

	// RTV用
	setting = DescriptorHeapSetting();
	setting.maxSize = 64;
	setting.heapType = DescriptorHeapSetting::RTV;
	DescriptorHeapManager::Create(setting, "RTV");

	// DSV用
	setting = DescriptorHeapSetting();
	setting.maxSize = 64;
	setting.heapType = DescriptorHeapSetting::DSV;
	DescriptorHeapManager::Create(setting, "DSV");
}
void RenderBase::CommandInit()
{
	HRESULT result;

	// コマンドアロケータの作成
	result = mDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&mCommandAllocator)
	);
	assert(SUCCEEDED(result));

	// コマンドリストの作成
	result = mDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCommandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&mCommandList)
	);
	assert(SUCCEEDED(result));

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC mCommandQueueDesc{};
	result = mDevice.Get()->CreateCommandQueue(&mCommandQueueDesc, IID_PPV_ARGS(&mCommandQueue));
	assert(SUCCEEDED(result));
}
void RenderBase::SwapChainInit()
{
	HRESULT result;

	// スワップチェインの初期化
	mBackBuffers[0] = std::make_unique<RenderTarget>();
	mBackBuffers[1] = std::make_unique<RenderTarget>();

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC1 mSwapChainDesc{};
	mSwapChainDesc.Width = (UINT)mRenderWindow->GetWindowSize().x;
	mSwapChainDesc.Height = (UINT)mRenderWindow->GetWindowSize().y;
	mSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色のフォーマット
	mSwapChainDesc.SampleDesc.Count = 1; // マルチサンプリングの設定
	mSwapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファとして使用
	mSwapChainDesc.BufferCount = 2; // ダブルバッファリング
	mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップモード
	mSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェインの作成
	ComPtr<IDXGISwapChain1> mSwapChain1;
	result = mDxgiFactory->CreateSwapChainForHwnd(
		mCommandQueue.Get(),
		mRenderWindow->GetHwnd(),
		&mSwapChainDesc,
		nullptr,
		nullptr,
		&mSwapChain1
	);
	mSwapChain1.As(&mSwapChain);
	assert(SUCCEEDED(result));

	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// レンダーターゲットビューのフォーマットを設定
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// バックバッファに対する設定
	for (size_t i = 0; i < mBackBuffers.size(); i++)
	{
		// バックバッファを取得
		mSwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(mBackBuffers[i]->GetBufferResource()->buffer.GetAddressOf()));

		// レンダーターゲットビューを作成
		DescriptorHeapManager::GetDescriptorHeap("RTV")->CreateRTV(mBackBuffers[i]->GetBufferResource());
	}
}
void RenderBase::FenceInit()
{
	HRESULT result;

	// フェンスの初期化
	result = mDevice->CreateFence(
		mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
}
void RenderBase::DepthBufferInit()
{
	// 深度バッファの初期化
	mDepthBuffer = std::make_unique<DepthBuffer>();
	mDepthBuffer->Create(RenderWindow::GetInstance()->GetWindowSize());
	DescriptorHeapManager::GetDescriptorHeap("DSV")->CreateDSV(mDepthBuffer->GetBufferResource());
}
void RenderBase::ShaderCompilerInit()
{
	std::string path1 = "NewEngine/Shader/";

	ShaderCompilerSetting setting;

	// Object3D用
	ShaderCompilerManager::Create(path1 + "Object3D", "Object3D");

	// Fbxモデル用
	ShaderCompilerManager::Create(path1 + "FbxModel", "FbxModel");

	// スプライト用
	ShaderCompilerManager::Create(path1 + "Sprite", "Sprite");

	// 円ゲージスプライト用
	ShaderCompilerManager::Create(path1 + "CircleGaugeSprite", "CircleGaugeSprite");

	// ポストエフェクト用（デフォルトシェーダー）
	ShaderCompilerManager::Create(path1 + "PostEffect", "PostEffect");

	// 線用
	ShaderCompilerManager::Create(path1 + "Line", "Line");

	// エミッター用
	ShaderCompilerManager::Create(path1 + "Emitter", "Emitter");

	// GPUエミッター用
	ShaderCompilerManager::Create(path1 + "GPUEmitter", "GPUEmitter");

	// ColliderObject用
	ShaderCompilerManager::Create(path1 + "ColliderObject", "ColliderObject");

	// ParticleMesh用
	ShaderCompilerManager::Create(path1 + "ParticleMesh", "ParticleMesh");

	// ParticleObject用
	ShaderCompilerManager::Create(path1 + "ParticleObject", "ParticleObject");

	// ボリューメトリックフォグ用
	ShaderCompilerManager::Create(path1 + "VolumetricFog", "VolumetricFog");

	// トーンマッピング用
	ShaderCompilerManager::Create(path1 + "Graphics/" + "ToonMapping", "ToonMapping");
}
void RenderBase::GraphicsPipelineInit()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc1{};
	depthStencilDesc1.DepthEnable = true;
	depthStencilDesc1.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc1.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc2{};
	depthStencilDesc2.DepthEnable = false;

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc3{};
	depthStencilDesc3.DepthEnable = true;
	depthStencilDesc3.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc3.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc4{};
	depthStencilDesc4.DepthEnable = true;
	depthStencilDesc4.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc4.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	GraphicsPipelineSetting setting;

	// 3Dオブジェクト用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Object3D");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 9;
	setting.rootSignatureSetting.maxSrvDescritorRange = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "Object3D");

	// FBXモデル用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("FbxModel");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "FbxModel");

	// スプライト用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Sprite");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Sprite");

	// 円ゲージスプライト用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("CircleGaugeSprite");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "CircleGaugeSprite");

	// ポストエフェクト用(デフォルト)
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("PostEffect");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "PostEffect");

	// 線用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Line");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::LineStrip;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "Line");

	// エミッタ用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Emitter");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Emitter");

	// GPUエミッタ用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("GPUEmitter");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "GPUEmitter");

	// ColliderObject用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ColliderObject");
	setting.cullMode = CullMode::None;
	setting.fillMode = GraphicsPipelineSetting::Wireframe;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "ColliderObject");

	// ParticleMesh用
	setting = GraphicsPipelineSetting();
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ParticleMesh");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc4;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "ParticleMesh");

	// ParticleObject用
	setting = GraphicsPipelineSetting();
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ParticleObject");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc4;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "ParticleObject");

	// ボリューメトリックフォグ用
	setting = GraphicsPipelineSetting();
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("VolumetricFog");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc4;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 6;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "VolumetricFog");

	setting = PipelineManager::GetGraphicsPipeline("VolumetricFog")->GetSetting();
	setting.depthStencilDesc = depthStencilDesc2;
	PipelineManager::CreateGraphicsPipeline(setting, "VolumetricFogInSide");

	//setting = PipelineManager::GetGraphicsPipeline("WithInVolumetricFog")->GetSetting();
	//setting.depthStencilDesc = depthStencilDesc2;
	//PipelineManager::CreateGraphicsPipeline(setting, "WithOutVolumetricFog");
}
void RenderBase::ComputePipelineInit()
{
	ComputePipelineSetting setting;

	// GPUエミッター用
	setting = ComputePipelineSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("GPUEmitter");
	setting.rootSignatureSetting.maxCbvRootParameter = 0;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "GPUEmitter");

	// ParticleMesh用
	setting = ComputePipelineSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ParticleMesh");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "ParticleMesh");

	// ParticleObject用
	setting = ComputePipelineSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ParticleObject");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "ParticleObject");
}

// --- ゲッター -------------------------------------------------------------- //
ID3D12Device* RenderBase::GetDevice() const
{
	return mDevice.Get();
}
ID3D12GraphicsCommandList* RenderBase::GetCommandList() const
{
	return mCommandList.Get();
}
ID3D12CommandQueue* RenderBase::GetCommandQueue() const
{
	return mCommandQueue.Get();
}
ID3D12CommandAllocator* RenderBase::GetCommandAllocator() const
{
	return mCommandAllocator.Get();
}
ID3D12Fence* RenderBase::GetFence() const
{
	return mFence.Get();
}
DepthBuffer* RenderBase::GetDepthBuffer() const
{
	return mDepthBuffer.get();
}
Viewport* RenderBase::GetViewport() const
{
	return mViewport.get();
}
UINT64 RenderBase::GetFenceValue() const
{
	return mFenceValue;
}