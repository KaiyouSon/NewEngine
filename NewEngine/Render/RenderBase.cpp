#include "RenderBase.h"
#include "RenderWindow.h"
#include "Viewport.h"
#include "ScissorRectangle.h"
#include "TextureManager.h"
#include <cassert>
#include <string>
#include <d3dcompiler.h>
#include <d3dx12.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
using namespace Microsoft::WRL;

float RenderBase::clearColor[4] = { 0.1f,0.25f,0.5f,0.0f };

void RenderBase::Init()
{
	renderWindow = RenderWindow::GetInstance().get();
	viewport = std::move(std::make_unique<Viewport>());
	scissorRectangle = std::move(std::make_unique<ScissorRectangle>());

	rtvIncrementIndex = 0;
	dsvIncrementIndex = 0;

	DeviceInit();			// デバイスの初期化
	DescriptorHeapInit();	// ティスクリプターヒープの初期化
	CommandInit();			// コマンド関連の初期化
	SwapChainInit();		// スワップチェンの初期化
	FenceInit();			// フェンスの初期化
	DepthBufferInit();		// 深度バッファの初期化
	ShaderCompilerInit();	// シェーダーコンパイラーの初期化
	RootSignatureInit();	// ルードシグネチャーの初期化
	GraphicsPipelineInit();	// グラフィックスパイプラインの初期化
}
void RenderBase::PreDraw()
{
	//---------------------- リソースバリアの変更コマンド ----------------------//
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// １．リソースバリアで書き込み可能に変更
	barrierDesc.Transition.pResource = backBuffers[bbIndex]->GetBuffer();	// バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	// 表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
	commandList->ResourceBarrier(1, &barrierDesc);

	//--------------------------- 描画先指定コマンド ---------------------------//
	// ２．描画先の変更
	// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = backBuffers[bbIndex]->GetCpuHandle();

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthBuffer->GetCpuHandle();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// 画面クリア R G B A
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// 深度バッファクリア
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポートの処理
	viewport->SetViewport(
		{ 0,0 },
		{
			(float)renderWindow->GetWindowSize().x,
			(float)renderWindow->GetWindowSize().y
		});
	viewport->Update();

	// シザー矩形の処理
	scissorRectangle->Update();

	//commandList->SetDescriptorHeaps(1, srvDescHeap.GetAddressOf());


	// SRVヒープの設定コマンド
	//auto temp = renderBase->GetSrvDescHeap();
}
void RenderBase::PostDraw()
{
	HRESULT result;

	//---------------------- リソースバリアの復帰コマンド ----------------------//
	// ５．リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
	commandList->ResourceBarrier(1, &barrierDesc);

	//-------------------------- コマンドのフラッシュ --------------------------//
	// 命令のクローズ
	result = commandList->Close();
	assert(SUCCEEDED(result));
	// コマンドリストの実行
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);

	// 画面に表示するバッファをフリップ（裏表の入替え）
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	// コマンドの実行完了を待つ
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence.Get()->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// キューをクリア
	result = commandAllocator->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
	result = commandList.Get()->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}
void RenderBase::SetObject3DDrawCommand()
{
	commandList->SetGraphicsRootSignature(object3DRootSignature->GetRootSignature());
}
void RenderBase::SetSpriteDrawCommand()
{
	commandList->SetGraphicsRootSignature(spriteRootSignature->GetRootSignature());
}

void RenderBase::CreateRTV(RenderTarget& renderTarget, const D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	// RTVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();

	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	rtvCpuHandle.ptr += descriptorSize * rtvIncrementIndex;

	renderTarget.SetCpuHandle(rtvCpuHandle);

	// ハンドルの指す位置にRTV作成
	device->CreateRenderTargetView(renderTarget.GetBuffer(), rtvDesc, rtvCpuHandle);

	rtvIncrementIndex++;
}
void RenderBase::CreateDSV(DepthBuffer& depthBuffer)
{
	// RTVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = dsvDescHeap->GetCPUDescriptorHandleForHeapStart();

	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	dsvCpuHandle.ptr += descriptorSize * dsvIncrementIndex;

	depthBuffer.SetCpuHandle(dsvCpuHandle);

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	// 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	// ハンドルの指す位置にRTV作成
	device->CreateDepthStencilView(depthBuffer.GetBuffer(), &dsvDesc, dsvCpuHandle);

	dsvIncrementIndex++;
}

void RenderBase::DeviceInit()
{
	HRESULT result;

	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
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
			IID_PPV_ARGS(device.GetAddressOf()));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

}
void RenderBase::DescriptorHeapInit()
{
	HRESULT result;

	TextureManager::CreateDescriptorHeap();

	// --- RTV ------------------------------------------------------ //
	const size_t maxRTVCount = 64;	// RTVの最大個数

	// RTV用デスクリプタヒープの設定
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// レンダーターゲットビュー
	//rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
	rtvHeapDesc.NumDescriptors = maxRTVCount; // 裏表の２つ

	// RTV用デスクリプタヒープの生成
	result = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescHeap));
	assert(SUCCEEDED(result));


	// --- DSV ------------------------------------------------------ //
	const size_t maxDSVCount = 64;	// DSVの最大個数

	// DSV用デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;		// デプスステンシルビュー
	//dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
	dsvHeapDesc.NumDescriptors = maxDSVCount;	// 深度ビューは一つ

	// DSV用デスクリプタヒープの生成
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvDescHeap));
	assert(SUCCEEDED(result));
}
void RenderBase::CommandInit()
{
	HRESULT result;

	// コマンドアロケータを生成
	result = device->CreateCommandAllocator
	(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator)
	);
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = device->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList)
	);
	assert(SUCCEEDED(result));

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result = device.Get()->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}
void RenderBase::SwapChainInit()
{
	HRESULT result;

	backBuffers[0] = std::move(std::make_unique<RenderTarget>());
	backBuffers[1] = std::move(std::make_unique<RenderTarget>());

	// リソースの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = (UINT)renderWindow->GetWindowSize().x;
	swapChainDesc.Height = (UINT)renderWindow->GetWindowSize().y;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				 // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1;								 // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				 // バックバッファ用
	swapChainDesc.BufferCount = 2;									 // バッファ数を２つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		 // フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェーンの生成
	ComPtr<IDXGISwapChain1> swapChain1;
	result = dxgiFactory->CreateSwapChainForHwnd
	(
		commandQueue.Get(),
		renderWindow->GetHwnd(),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain1
	);
	swapChain1.As(&swapChain);
	assert(SUCCEEDED(result));

	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(backBuffers[i]->GetBufferAddress()));

		CreateRTV(*backBuffers[i], &rtvDesc);

		//D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = rtvSwapChainDescHeap->GetCPUDescriptorHandleForHeapStart();
		//rtvCpuHandle.ptr += i * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		//backBuffers[i]->SetCpuHandle(rtvCpuHandle);

		//device->CreateRenderTargetView(backBuffers[i]->GetBuffer(), &rtvDesc, rtvCpuHandle);
	}
}
void RenderBase::FenceInit()
{
	HRESULT result;

	// フェンスの生成
	result = device->CreateFence(
		fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
}
void RenderBase::DepthBufferInit()
{
	HRESULT result;

	depthBuffer = std::move(std::make_unique<DepthBuffer>());
	depthBuffer->Create();
}
void RenderBase::ShaderCompilerInit()
{
	std::string path = "NewEngine/Shader/";

	// スプライト用シェーダー
	spriteShader = std::move(std::make_unique<ShaderObject>());
	spriteShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	spriteShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	spriteShader->CompileVertexShader(path + "SpriteVS.hlsl", "main");
	spriteShader->CompilePixelShader(path + "SpritePS.hlsl", "main");

	// スプライト用シェーダー
	circleGaugeSpriteShader = std::move(std::make_unique<ShaderObject>());
	circleGaugeSpriteShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	circleGaugeSpriteShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	circleGaugeSpriteShader->CompileVertexShader(path + "CircleGaugeSpriteVS.hlsl", "main");
	circleGaugeSpriteShader->CompilePixelShader(path + "CircleGaugeSpritePS.hlsl", "main");

	// レンダーテクスチャーのシェーダー
	renderTextureShader = std::move(std::make_unique<ShaderObject>());
	renderTextureShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	renderTextureShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	renderTextureShader->CompileVertexShader(path + "RenderTextureVS.hlsl", "main");
	renderTextureShader->CompilePixelShader(path + "RenderTexturePS.hlsl", "main");

	// Object3D用シェーダー
	object3DShader = std::move(std::make_unique<ShaderObject>());
	object3DShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	object3DShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	object3DShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	object3DShader->CompileVertexShader(path + "Object3DVS.hlsl", "main");
	object3DShader->CompilePixelShader(path + "Object3DPS.hlsl", "main");

	// シルエット用シェーダー
	silhouetteShader = std::move(std::make_unique<ShaderObject>());
	silhouetteShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	silhouetteShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	silhouetteShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	silhouetteShader->CompileVertexShader(path + "SilhouetteVS.hlsl", "main");
	silhouetteShader->CompilePixelShader(path + "SilhouettePS.hlsl", "main");

	// アウトラインObject用シェーダー
	outlineShader = std::move(std::make_unique<ShaderObject>());
	outlineShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	outlineShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	outlineShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	outlineShader->CompileVertexShader(path + "OutLineVS.hlsl", "main");
	outlineShader->CompilePixelShader(path + "OutLinePS.hlsl", "main");

	// トゥーンレンダーリング用
	toonRenderShader = std::move(std::make_unique<ShaderObject>());
	toonRenderShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	toonRenderShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	toonRenderShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	toonRenderShader->CompileVertexShader(path + "ToonRenderVS.hlsl", "main");
	toonRenderShader->CompilePixelShader(path + "ToonRenderPS.hlsl", "main");

	// ライン用
	lineShader = std::move(std::make_unique<ShaderObject>());
	lineShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	lineShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	lineShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	lineShader->CompileVertexShader(path + "LineVS.hlsl", "main");
	lineShader->CompilePixelShader(path + "LinePS.hlsl", "main");
}
void RenderBase::RootSignatureInit()
{
	// 3Dオブジェクト用
	object3DRootSignature = std::move(std::make_unique<RootSignature>());
	object3DRootSignature->AddConstantBufferViewToRootRrameter(7);
	object3DRootSignature->Create();

	// スプライト用
	spriteRootSignature = std::move(std::make_unique<RootSignature>());
	spriteRootSignature->AddConstantBufferViewToRootRrameter(3);
	spriteRootSignature->Create();
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

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc4 = D3D12_DEPTH_STENCIL_DESC();
	depthStencilDesc4.DepthEnable = true; // 深度テストを行う
	depthStencilDesc4.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 書き込み許可
	depthStencilDesc4.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用

	// 3Dオブジェクト用
	GraphicsPipelineManager::Create(
		object3DShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullBack,
		depthStencilDesc1,
		TopologyType::TriangleTopology,
		"Object3D");

	// スプライト用
	GraphicsPipelineManager::Create(
		spriteShader.get(),
		spriteRootSignature->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::TriangleTopology,
		"Sprite");

	// 円形ゲージスプライト用
	GraphicsPipelineManager::Create(
		circleGaugeSpriteShader.get(),
		spriteRootSignature->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::TriangleTopology,
		"CircleGaugeSprite");

	// レンダーテクスチャ用
	GraphicsPipelineManager::Create(
		renderTextureShader.get(),
		spriteRootSignature->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::TriangleTopology,
		"RenderTexture");

	// シルエット用
	GraphicsPipelineManager::Create(
		silhouetteShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullBack,
		depthStencilDesc3,
		TopologyType::TriangleTopology,
		"Silhouette");

	// アウトライン用
	GraphicsPipelineManager::Create(
		outlineShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullFront,
		depthStencilDesc4,
		TopologyType::TriangleTopology,
		"Outline");

	// トゥーンレンダリング用
	GraphicsPipelineManager::Create(
		toonRenderShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullBack,
		depthStencilDesc1,
		TopologyType::TriangleTopology,
		"ToonRendering");

	// コライダー用
	GraphicsPipelineManager::Create(
		lineShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullBack,
		depthStencilDesc1,
		TopologyType::TriangleTopology,
		"Collider");

	// Line用
	linePipeline = std::move(std::make_unique<GraphicsPipeline>());
	linePipeline->SetShaderObject(object3DShader.get());
	linePipeline->SetCullMode(CullMode::None);
	linePipeline->SetDepthStencilDesc(depthStencilDesc1);
	linePipeline->SetTopologyType(TopologyType::LineTopology);
	linePipeline->SetRootSignature(object3DRootSignature->GetRootSignature());
	linePipeline->Create();
}