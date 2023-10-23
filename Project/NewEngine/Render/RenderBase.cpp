#include "RenderBase.h"
#include "RenderWindow.h"
#include "Viewport.h"
#include "ScissorRectangle.h"
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
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path1 + "Object3D/Object3DVS.hlsl";
	setting.psFilePath = path1 + "Object3D/Object3DPS.hlsl";
	ShaderCompilerManager::Create(setting, "Object3D");

	// Fbxモデル用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(5);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.mInputLayoutSettings[3] = InputLayoutSetting("BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT);
	setting.mInputLayoutSettings[4] = InputLayoutSetting("BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT);
	setting.vsFilePath = path1 + "FbxModel/FbxModelVS.hlsl";
	setting.psFilePath = path1 + "FbxModel/FbxModelPS.hlsl";
	ShaderCompilerManager::Create(setting, "FbxModel");

	// スプライト用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path1 + "Sprite/SpriteVS.hlsl";
	setting.psFilePath = path1 + "Sprite/SpritePS.hlsl";
	ShaderCompilerManager::Create(setting, "Sprite");

	// 円ゲージスプライト用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path1 + "CircleGaugeSprite/CircleGaugeSpriteVS.hlsl";
	setting.psFilePath = path1 + "CircleGaugeSprite/CircleGaugeSpritePS.hlsl";
	ShaderCompilerManager::Create(setting, "CircleGaugeSprite");

	// ポストエフェクト用（デフォルトシェーダー）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path1 + "PostEffect/PostEffectVS.hlsl";
	setting.psFilePath = path1 + "PostEffect/PostEffectPS.hlsl";
	ShaderCompilerManager::Create(setting, "PostEffect");

	// 線用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(1);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.vsFilePath = path1 + "Line/LineVS.hlsl";
	setting.psFilePath = path1 + "Line/LinePS.hlsl";
	ShaderCompilerManager::Create(setting, "Line");

	// エミッター用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(5);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);	// 座標
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);		// スケール
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 1);		// z軸回転
	setting.mInputLayoutSettings[3] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 2);		// 輝度
	setting.mInputLayoutSettings[4] = InputLayoutSetting("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);	// 色
	setting.vsFilePath = path1 + "Emitter/EmitterVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "Emitter");

	// GPUエミッター用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.clear();
	setting.csFilePath = path1 + "Emitter/EmitterCS.hlsl";
	setting.vsFilePath = path1 + "Emitter/GPUEmitterVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "GPUEmitter");

	// ColliderObject用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path1 + "ColliderObject/ColliderObjectVS.hlsl";
	setting.psFilePath = path1 + "ColliderObject/ColliderObjectPS.hlsl";
	ShaderCompilerManager::Create(setting, "ColliderObject");

	// ParticleMesh用
	setting = ShaderCompilerSetting();
	setting.csFilePath = path1 + "ParticleMesh/ParticleMeshCS.hlsl";
	setting.vsFilePath = path1 + "ParticleMesh/ParticleMeshVS.hlsl";
	setting.gsFilePath = path1 + "ParticleMesh/ParticleMeshGS.hlsl";
	setting.psFilePath = path1 + "ParticleMesh/ParticleMeshPS.hlsl";
	ShaderCompilerManager::Create(setting, "ParticleMesh");
}
void RenderBase::GraphicsPipelineInit()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc1{};
	depthStencilDesc1.DepthEnable = true; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧
	depthStencilDesc1.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 譖ｸ縺崎ｾｼ縺ｿ險ｱ蜿ｯ
	depthStencilDesc1.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 蟆上＆縺・⊇縺・ｒ謗｡逕ｨ

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc2{};
	depthStencilDesc2.DepthEnable = false; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後ｏ縺ｪ縺・

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc3{};
	depthStencilDesc3.DepthEnable = true; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧
	depthStencilDesc3.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// 譖ｸ縺崎ｾｼ縺ｿ荳榊庄
	depthStencilDesc3.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;	// 螟ｧ縺阪＞縺ｻ縺・ｒ謗｡逕ｨ

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc4{};
	depthStencilDesc4.DepthEnable = true; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧
	depthStencilDesc4.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// 譖ｸ縺崎ｾｼ縺ｿ險ｱ蜿ｯ
	depthStencilDesc4.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 蟆上＆縺・⊇縺・ｒ謗｡逕ｨ

	GraphicsPipelineSetting setting;

	// 3Dオブジェクト用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Object3D");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 8;
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
