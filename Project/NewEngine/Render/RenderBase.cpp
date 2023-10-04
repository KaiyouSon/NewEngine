#include "RenderBase.h"
#include "RenderWindow.h"
#include "Viewport.h"
#include "ScissorRectangle.h"
#include "TextureManager.h"
#include "ShaderObjectManager.h"
#include <cassert>
#include <string>
#include <d3dcompiler.h>
#include <d3dx12.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
using namespace Microsoft::WRL;

float RenderBase::sClearColor[4] = { 0.1f,0.25f,0.5f,0.0f };

void RenderBase::Init()
{
	// デバッグ時のみ実行
	ProcessAtDebugBulid([]()
		{
			//デバッグレイヤーをオンに
			ComPtr<ID3D12Debug1> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
			{
				debugController->EnableDebugLayer();
				debugController->SetEnableGPUBasedValidation(false);
			}
		});

	DeviceInit();	// デバイスの初期化

	// デバッグ時のみ実行
	ProcessAtDebugBulid([]()
		{
			ComPtr<ID3D12InfoQueue> infoQueue;
			if (SUCCEEDED(RenderBase::GetInstance()->
				GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue))))
			{
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	// やばいエラー一時に止まる
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		// エラー時に止まる
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	// ワーニング時に止まる
			}

			//　抑制するエラー
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
			};

			//　抑制される表示レベル
			D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
			D3D12_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = _countof(denyIds);
			filter.DenyList.pIDList = denyIds;
			filter.DenyList.NumSeverities = _countof(severities);
			filter.DenyList.pSeverityList = severities;
			//　指定したエラーの表示を抑制する
			infoQueue->PushStorageFilter(&filter);
		});

	mRenderWindow = RenderWindow::GetInstance().get();
	mViewport = std::make_unique<Viewport>();
	mScissorRectangle = std::make_unique<ScissorRectangle>();

	mFenceValue = 0;

	DescriptorHeapInit();	// ティスクリプターヒープの初期化
	CommandInit();			// コマンド関連の初期化
	SwapChainInit();		// スワップチェンの初期化
	FenceInit();			// フェンスの初期化
	DepthBufferInit();		// 深度バッファの初期化
	ShaderCompilerInit();	// シェーダーコンパイラーの初期化
	GraphicsPipelineInit();	// グラフィックスパイプラインの初期化
	ComputePipelineInit();	// コンピュートパイプラインの初期化
}
void RenderBase::PreDraw()
{
	//---------------------- リソースバリアの変更コマンド ----------------------//
	// バックバッファの番号を取得
	UINT bbIndex = mSwapChain->GetCurrentBackBufferIndex();
	// １．リソースバリアで書き込み可能に変更
	mBarrierDesc.Transition.pResource = mBackBuffers[bbIndex]->GetBufferResource()->buffer.Get();	// バックバッファを指定
	mBarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	// 表示状態から
	mBarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
	mCommandList->ResourceBarrier(1, &mBarrierDesc);

	//--------------------------- 描画先指定コマンド ---------------------------//
	// ２．描画先の変更
	// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mBackBuffers[bbIndex]->GetBufferResource()->rtvHandle.cpu;

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDepthBuffer->GetBufferResource()->dsvHandle.cpu;
	mCommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// 画面クリア R G B A
	mCommandList->ClearRenderTargetView(rtvHandle, sClearColor, 0, nullptr);

	// 深度バッファクリア
	mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポートの処理
	mViewport->SetViewport(
		{ 0,0 },
		{
			(float)mRenderWindow->GetWindowSize().x,
			(float)mRenderWindow->GetWindowSize().y
		});
	mViewport->Update();

	// シザー矩形の処理
	mScissorRectangle->Update();
}
void RenderBase::PostDraw()
{
	HRESULT result;

	//---------------------- リソースバリアの復帰コマンド ----------------------//
	// ５．リソースバリアを戻す
	mBarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
	mBarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
	mCommandList->ResourceBarrier(1, &mBarrierDesc);

	//-------------------------- コマンドのフラッシュ --------------------------//
	// 命令のクローズ
	result = mCommandList->Close();
	assert(SUCCEEDED(result));
	// コマンドリストの実行
	ID3D12CommandList* mCommandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(1, mCommandLists);

	// 画面に表示するバッファをフリップ（裏表の入替え）
	result = mSwapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	// コマンドの実行完了を待つ
	mCommandQueue->Signal(mFence.Get(), ++mFenceValue);
	if (mFence.Get()->GetCompletedValue() != mFenceValue)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// キューをクリア
	result = mCommandAllocator->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
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
	mCommandList->ResourceBarrier(1, &barrier);
}

// --- 初期化関連 ------------------------------------------------------------ //
void RenderBase::DeviceInit()
{
	HRESULT result;

	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&mDxgiFactory));
	assert(SUCCEEDED(result));

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		mDxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i].Get();
			break;
		}
	}

	// 対応レベルの配列
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
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(mDevice.GetAddressOf()));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

}
void RenderBase::DescriptorHeapInit()
{
	DescriptorHeapSetting setting;
	// SRV格納用
	setting.maxSize = 2048;
	setting.startIndex = 1;
	setting.heapType = DescriptorHeapSetting::CBV_SRV_UAV;
	DescriptorHeapManager::Create(setting, "SRV");

	// RTV格納用
	setting.maxSize = 64;
	setting.heapType = DescriptorHeapSetting::RTV;
	DescriptorHeapManager::Create(setting, "RTV");

	// DSV格納用
	setting.maxSize = 64;
	setting.heapType = DescriptorHeapSetting::DSV;
	DescriptorHeapManager::Create(setting, "DSV");

	// SRV_UAV格納用
	setting.maxSize = 512;
	setting.heapType = DescriptorHeapSetting::CBV_SRV_UAV;
	DescriptorHeapManager::Create(setting, "SRV_UAV");
}
void RenderBase::CommandInit()
{
	HRESULT result;

	// コマンドアロケータを生成
	result = mDevice->CreateCommandAllocator
	(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&mCommandAllocator)
	);
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = mDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCommandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&mCommandList)
	);
	assert(SUCCEEDED(result));

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC mCommandQueueDesc{};
	//コマンドキューを生成
	result = mDevice.Get()->CreateCommandQueue(&mCommandQueueDesc, IID_PPV_ARGS(&mCommandQueue));
	assert(SUCCEEDED(result));
}
void RenderBase::SwapChainInit()
{
	HRESULT result;

	mBackBuffers[0] = std::make_unique<RenderTarget>();
	mBackBuffers[1] = std::make_unique<RenderTarget>();

	// リソースの設定
	DXGI_SWAP_CHAIN_DESC1 mSwapChainDesc{};
	mSwapChainDesc.Width = (UINT)mRenderWindow->GetWindowSize().x;
	mSwapChainDesc.Height = (UINT)mRenderWindow->GetWindowSize().y;
	mSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				 // 色情報の書式
	mSwapChainDesc.SampleDesc.Count = 1;							 // マルチサンプルしない
	mSwapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;			 // バックバッファ用
	mSwapChainDesc.BufferCount = 2;									 // バッファ数を２つに設定
	mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		 // フリップ後は破棄
	mSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェーンの生成
	ComPtr<IDXGISwapChain1> mSwapChain1;
	result = mDxgiFactory->CreateSwapChainForHwnd
	(
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
	// シェーダーの計算結果をSRGBに変換して書き込む
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < mBackBuffers.size(); i++)
	{
		// スワップチェーンからバッファを取得
		mSwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(mBackBuffers[i]->GetBufferResource()->buffer.GetAddressOf()));

		// RTV作成
		DescriptorHeapManager::GetDescriptorHeap("RTV")->CreateRTV(mBackBuffers[i]->GetBufferResource());
	}
}
void RenderBase::FenceInit()
{
	HRESULT result;

	// フェンスの生成
	result = mDevice->CreateFence(
		mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
}
void RenderBase::DepthBufferInit()
{
	mDepthBuffer = std::make_unique<DepthBuffer>();
	mDepthBuffer->Create();
	DescriptorHeapManager::GetDescriptorHeap("DSV")->CreateDSV(mDepthBuffer->GetBufferResource());
}
void RenderBase::ShaderCompilerInit()
{
	std::string path1 = "NewEngine/Shader/";
	std::string path2 = "Application/Shader/";

	// Object3D用シェーダー
	ShaderObjectManager::Create("Object3D");
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->CompileVertexShader(path1 + "Object3DVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Object3D")->CompilePixelShader(path1 + "Object3DPS.hlsl", "main");

	// Fbxモデル用シェーダー
	ShaderObjectManager::Create("FbxModel");
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->CompileVertexShader(path1 + "FbxModelVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("FbxModel")->CompilePixelShader(path1 + "FbxModelPS.hlsl", "main");

	// スプライト用シェーダー
	ShaderObjectManager::Create("Sprite");
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->CompileVertexShader(path1 + "SpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Sprite")->CompilePixelShader(path1 + "SpritePS.hlsl", "main");

	// 円ゲージスプライト用シェーダー
	ShaderObjectManager::Create("CircleGaugeSprite");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompileVertexShader(path1 + "CircleGaugeSpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompilePixelShader(path1 + "CircleGaugeSpritePS.hlsl", "main");

	// レンダーテクスチャーのシェーダー
	ShaderObjectManager::Create("RenderTexture");
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompileVertexShader(path1 + "RenderTextureVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompilePixelShader(path1 + "RenderTexturePS.hlsl", "main");

	// シルエット用シェーダー
	ShaderObjectManager::Create("Silhouette");
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->CompileVertexShader(path1 + "SilhouetteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Silhouette")->CompilePixelShader(path1 + "SilhouettePS.hlsl", "main");

	// アウトラインObject用シェーダー
	ShaderObjectManager::Create("Outline");
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->CompileVertexShader(path1 + "OutLineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Outline")->CompilePixelShader(path1 + "OutLinePS.hlsl", "main");

	// トゥーンレンダーリング用
	ShaderObjectManager::Create("ToonRendering");
	ShaderObjectManager::GetShaderObject("ToonRendering")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRendering")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRendering")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRendering")->CompileVertexShader(path1 + "ToonRenderVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ToonRendering")->CompilePixelShader(path1 + "ToonRenderPS.hlsl", "main");

	// ライン用
	ShaderObjectManager::Create("Line");
	ShaderObjectManager::GetShaderObject("Line")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Line")->CompileVertexShader(path1 + "LineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Line")->CompilePixelShader(path1 + "LinePS.hlsl", "main");

	// エミッター用
	ShaderObjectManager::Create("Emitter");
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);	// 座標
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);		// スケール
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 1);		// 回転
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 2);		// 輝き度
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);	// 色
	ShaderObjectManager::GetShaderObject("Emitter")->CompileComputeShader(path1 + "EmitterCS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Emitter")->CompileVertexShader(path1 + "EmitterVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Emitter")->CompileGeometryShader(path1 + "EmitterGS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Emitter")->CompilePixelShader(path1 + "EmitterPS.hlsl", "main");

	// GPUエミッター用
	ShaderObjectManager::Create("GPUEmitter");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompileComputeShader(path2 + "RespawnPointEffectCS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompileVertexShader(path2 + "RespawnPointEffectVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompileGeometryShader(path1 + "EmitterGS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompilePixelShader(path1 + "EmitterPS.hlsl", "main");

	// ColliderObject用シェーダー
	ShaderObjectManager::Create("ColliderObject");
	ShaderObjectManager::GetShaderObject("ColliderObject")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ColliderObject")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ColliderObject")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ColliderObject")->CompileVertexShader(path1 + "ColliderObjectVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ColliderObject")->CompilePixelShader(path1 + "ColliderObjectPS.hlsl", "main");
}
void RenderBase::GraphicsPipelineInit()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc1{};
	depthStencilDesc1.DepthEnable = true; // 深度テストを行う
	depthStencilDesc1.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 書き込み許可
	depthStencilDesc1.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc2{};
	depthStencilDesc2.DepthEnable = false; // 深度テストを行わない

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc3{};
	depthStencilDesc3.DepthEnable = true; // 深度テストを行う
	depthStencilDesc3.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// 書き込み不可
	depthStencilDesc3.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;	// 大きいほうを採用

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc4{};
	depthStencilDesc4.DepthEnable = true; // 深度テストを行う
	depthStencilDesc4.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 書き込み許可
	depthStencilDesc4.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用

	GraphicsPipelineSetting setting;

	// 3Dオブジェクト用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Object3D");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 8;
	setting.rootSignatureSetting.maxSrvDescritorRange = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "Object3D");

	// FBXモデル用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("FbxModel");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "FbxModel");

	// スプライト用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Sprite");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Sprite");

	// 円形ゲージスプライト用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("CircleGaugeSprite");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "CircleGaugeSprite");

	// レンダーテクスチャ用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("RenderTexture");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "RenderTexture");

	// シルエット用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Silhouette");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc3;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Silhouette");

	// アウトライン用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Outline");
	setting.cullMode = CullMode::Front;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc4;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Outline");

	// トゥーンレンダリング用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ToonRendering");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc4;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "ToonRendering");

	// ライン用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Line");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::LineStrip;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "Line");

	// エミッター用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Emitter");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Emitter");

	// GPUエミッター用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("GPUEmitter");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "GPUEmitter");

	// ColliderObject用
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ColliderObject");
	setting.cullMode = CullMode::None;
	setting.fillMode = GraphicsPipelineSetting::Wireframe;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "ColliderObject");
}
void RenderBase::ComputePipelineInit()
{
	ComputePipelineSetting setting;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("GPUEmitter");
	setting.rootSignatureSetting.maxCbvRootParameter = 0;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "GPUEmitter");
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