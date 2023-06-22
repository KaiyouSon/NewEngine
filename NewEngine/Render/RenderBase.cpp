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

float RenderBase::sClearColor_[4] = { 0.1f,0.25f,0.5f,0.0f };

void RenderBase::Init()
{
	renderWindow_ = RenderWindow::GetInstance().get();
	viewport_ = std::make_unique<Viewport>();
	scissorRectangle_ = std::make_unique<ScissorRectangle>();

	rtvIncrementIndex_ = 0;
	dsvIncrementIndex_ = 0;

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
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();
	// １．リソースバリアで書き込み可能に変更
	barrierDesc_.Transition.pResource = backBuffers_[bbIndex]->GetBuffer();	// バックバッファを指定
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	// 表示状態から
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
	commandList_->ResourceBarrier(1, &barrierDesc_);

	//--------------------------- 描画先指定コマンド ---------------------------//
	// ２．描画先の変更
	// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = backBuffers_[bbIndex]->GetCpuHandle();

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthBuffer_->GetCpuHandle();
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// 画面クリア R G B A
	commandList_->ClearRenderTargetView(rtvHandle, sClearColor_, 0, nullptr);

	// 深度バッファクリア
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポートの処理
	viewport_->SetViewport(
		{ 0,0 },
		{
			(float)renderWindow_->GetWindowSize().x,
			(float)renderWindow_->GetWindowSize().y
		});
	viewport_->Update();

	// シザー矩形の処理
	scissorRectangle_->Update();

	//commandList_->SetDescriptorHeaps(1, srvDescHeap.GetAddressOf());


	// SRVヒープの設定コマンド
	//auto temp = renderBase->GetSrvDescHeap();
}
void RenderBase::PostDraw()
{
	HRESULT result;

	//---------------------- リソースバリアの復帰コマンド ----------------------//
	// ５．リソースバリアを戻す
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
	commandList_->ResourceBarrier(1, &barrierDesc_);

	//-------------------------- コマンドのフラッシュ --------------------------//
	// 命令のクローズ
	result = commandList_->Close();
	assert(SUCCEEDED(result));
	// コマンドリストの実行
	ID3D12CommandList* commandList_s[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandList_s);

	// 画面に表示するバッファをフリップ（裏表の入替え）
	result = swapChain_->Present(1, 0);
	assert(SUCCEEDED(result));

	// コマンドの実行完了を待つ
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_.Get()->GetCompletedValue() != fenceVal_)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// キューをクリア
	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
	result = commandList_.Get()->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result));
}
void RenderBase::SetObject3DDrawCommand()
{
	commandList_->SetGraphicsRootSignature(object3DRootSignature_->GetRootSignature());
}
void RenderBase::SetSpriteDrawCommand()
{
	commandList_->SetGraphicsRootSignature(spriteRootSignature_->GetRootSignature());
}
void RenderBase::SetRenderTextureDrawCommand()
{
	commandList_->SetGraphicsRootSignature(renderTextureRootSignature_->GetRootSignature());
}

void RenderBase::CreateRTV(RenderTarget& renderTarget, const D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	// RTVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = rtvDescHeap_->GetCPUDescriptorHandleForHeapStart();

	UINT descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	rtvCpuHandle.ptr += descriptorSize * rtvIncrementIndex_;

	renderTarget.SetCpuHandle(rtvCpuHandle);

	// ハンドルの指す位置にRTV作成
	device_->CreateRenderTargetView(renderTarget.GetBuffer(), rtvDesc, rtvCpuHandle);

	rtvIncrementIndex_++;
}
void RenderBase::CreateDSV(DepthBuffer& depthBuffer_)
{
	// RTVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = dsvDescHeap_->GetCPUDescriptorHandleForHeapStart();

	UINT descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	dsvCpuHandle.ptr += descriptorSize * dsvIncrementIndex_;

	depthBuffer_.SetCpuHandle(dsvCpuHandle);

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	// 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	// ハンドルの指す位置にRTV作成
	device_->CreateDepthStencilView(depthBuffer_.GetBuffer(), &dsvDesc, dsvCpuHandle);

	dsvIncrementIndex_++;
}

void RenderBase::DeviceInit()
{
	HRESULT result;

	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i,
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
			IID_PPV_ARGS(device_.GetAddressOf()));
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
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// レンダーターゲットビュー
	//rtvHeapDesc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
	rtvHeapDesc_.NumDescriptors = maxRTVCount; // 裏表の２つ

	// RTV用デスクリプタヒープの生成
	result = device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvDescHeap_));
	assert(SUCCEEDED(result));


	// --- DSV ------------------------------------------------------ //
	const size_t maxDSVCount = 64;	// DSVの最大個数

	// DSV用デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;		// デプスステンシルビュー
	//dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
	dsvHeapDesc.NumDescriptors = maxDSVCount;	// 深度ビューは一つ

	// DSV用デスクリプタヒープの生成
	result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvDescHeap_));
	assert(SUCCEEDED(result));
}
void RenderBase::CommandInit()
{
	HRESULT result;

	// コマンドアロケータを生成
	result = device_->CreateCommandAllocator
	(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_)
	);
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = device_->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_)
	);
	assert(SUCCEEDED(result));

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueue_Desc{};
	//コマンドキューを生成
	result = device_.Get()->CreateCommandQueue(&commandQueue_Desc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));
}
void RenderBase::SwapChainInit()
{
	HRESULT result;

	backBuffers_[0] = std::make_unique<RenderTarget>();
	backBuffers_[1] = std::make_unique<RenderTarget>();

	// リソースの設定
	DXGI_SWAP_CHAIN_DESC1 swapChain_Desc{};
	swapChain_Desc.Width = (UINT)renderWindow_->GetWindowSize().x;
	swapChain_Desc.Height = (UINT)renderWindow_->GetWindowSize().y;
	swapChain_Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				 // 色情報の書式
	swapChain_Desc.SampleDesc.Count = 1;								 // マルチサンプルしない
	swapChain_Desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				 // バックバッファ用
	swapChain_Desc.BufferCount = 2;									 // バッファ数を２つに設定
	swapChain_Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		 // フリップ後は破棄
	swapChain_Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェーンの生成
	ComPtr<IDXGISwapChain1> swapChain_1;
	result = dxgiFactory_->CreateSwapChainForHwnd
	(
		commandQueue_.Get(),
		renderWindow_->GetHwnd(),
		&swapChain_Desc,
		nullptr,
		nullptr,
		&swapChain_1
	);
	swapChain_1.As(&swapChain_);
	assert(SUCCEEDED(result));

	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers_.size(); i++)
	{
		// スワップチェーンからバッファを取得
		swapChain_->GetBuffer((UINT)i, IID_PPV_ARGS(backBuffers_[i]->GetBufferAddress()));

		CreateRTV(*backBuffers_[i], &rtvDesc);
	}
}
void RenderBase::FenceInit()
{
	HRESULT result;

	// フェンスの生成
	result = device_->CreateFence(
		fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence_.GetAddressOf()));
}
void RenderBase::DepthBufferInit()
{
	depthBuffer_ = std::make_unique<DepthBuffer>();
	depthBuffer_->Create();
}
void RenderBase::ShaderCompilerInit()
{
	std::string path = "NewEngine/Shader/";

	// Object3D用シェーダー
	ShaderObjectManager::Create("Object3D");
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->CompileVertexShader(path + "Object3DVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Object3D")->CompilePixelShader(path + "Object3DPS.hlsl", "main");

	// Fbxモデル用シェーダー
	ShaderObjectManager::Create("FbxModel");
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->CompileVertexShader(path + "FbxModelVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("FbxModel")->CompilePixelShader(path + "FbxModelPS.hlsl", "main");

	// スプライト用シェーダー
	ShaderObjectManager::Create("Sprite");
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->CompileVertexShader(path + "SpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Sprite")->CompilePixelShader(path + "SpritePS.hlsl", "main");

	// 円ゲージスプライト用シェーダー
	ShaderObjectManager::Create("CircleGaugeSprite");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompileVertexShader(path + "CircleGaugeSpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompilePixelShader(path + "CircleGaugeSpritePS.hlsl", "main");

	// レンダーテクスチャーのシェーダー
	ShaderObjectManager::Create("RenderTexture");
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompileVertexShader(path + "RenderTextureVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompilePixelShader(path + "RenderTexturePS.hlsl", "main");

	// シルエット用シェーダー
	ShaderObjectManager::Create("Silhouette");
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->CompileVertexShader(path + "SilhouetteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Silhouette")->CompilePixelShader(path + "SilhouettePS.hlsl", "main");

	// アウトラインObject用シェーダー
	ShaderObjectManager::Create("Outline");
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->CompileVertexShader(path + "OutLineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Outline")->CompilePixelShader(path + "OutLinePS.hlsl", "main");

	// トゥーンレンダーリング用
	ShaderObjectManager::Create("ToonRender");
	ShaderObjectManager::GetShaderObject("ToonRender")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRender")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRender")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRender")->CompileVertexShader(path + "ToonRenderVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ToonRender")->CompilePixelShader(path + "ToonRenderPS.hlsl", "main");

	// ライン用
	ShaderObjectManager::Create("Line");
	ShaderObjectManager::GetShaderObject("Line")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Line")->CompileVertexShader(path + "LineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Line")->CompilePixelShader(path + "LinePS.hlsl", "main");
}
void RenderBase::RootSignatureInit()
{
	// 3Dオブジェクト用
	object3DRootSignature_ = std::make_unique<RootSignature>();
	object3DRootSignature_->AddConstantBufferViewToRootRrameter(7);
	object3DRootSignature_->AddDescriptorRangeToRootPrameter(1);
	object3DRootSignature_->Create(1);

	// スプライト用
	spriteRootSignature_ = std::make_unique<RootSignature>();
	spriteRootSignature_->AddConstantBufferViewToRootRrameter(3);
	spriteRootSignature_->AddDescriptorRangeToRootPrameter(1);
	spriteRootSignature_->Create(1);

	// スプライト用
	renderTextureRootSignature_ = std::make_unique<RootSignature>();
	renderTextureRootSignature_->AddConstantBufferViewToRootRrameter(5);
	renderTextureRootSignature_->AddDescriptorRangeToRootPrameter(1);
	renderTextureRootSignature_->Create(2);
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

	// 3Dオブジェクト用
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Object3D"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Back,
		depthStencilDesc1,
		TopologyType::Triangle,
		2,
		"Object3D");

	// 3Dオブジェクト用
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("FbxModel"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Back,
		depthStencilDesc1,
		TopologyType::Triangle,
		1,
		"FbxModel");

	// スプライト用
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Sprite"),
		spriteRootSignature_->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::Triangle,
		2,
		"Sprite");

	// 円形ゲージスプライト用
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("CircleGaugeSprite"),
		spriteRootSignature_->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::Triangle,
		1,
		"CircleGaugeSprite");

	// レンダーテクスチャ用
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("RenderTexture"),
		renderTextureRootSignature_->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::Triangle,
		2,
		"RenderTexture");

	// シルエット用
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Silhouette"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Back,
		depthStencilDesc3,
		TopologyType::Triangle,
		1,
		"Silhouette");

	// アウトライン用
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Outline"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Front,
		depthStencilDesc4,
		TopologyType::Triangle,
		1,
		"Outline");

	// トゥーンレンダリング用
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("ToonRender"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Back,
		depthStencilDesc1,
		TopologyType::Triangle,
		1,
		"ToonRendering");

	// ライン用
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Line"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::None,
		depthStencilDesc1,
		TopologyType::Line,
		1,
		"Line");
}