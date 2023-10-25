#pragma once
#include "RenderWindow.h"
#include "ShaderCompiler.h"
#include "RootSignature.h"
#include "PipelineManager.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "DepthBuffer.h"
#include "IConstantBuffer.h"
#include "BufferResource.h"
#include "Viewport.h"
#include "ScissorRectangle.h"
#include "DescriptorHeapManager.h"
#include "NewEngineDefine.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include <memory>
#include <array>

// ゲームの描画ベースを管理するクラス
class RenderBase
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr <T>;
	static float sClearColor[4];

private:
	// デバイスとファクトリ
	ComPtr<ID3D12Device> mDevice;
	ComPtr<IDXGIFactory7> mDxgiFactory;

	// コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12CommandQueue> mCommandQueue;

	// スワップチェイン
	ComPtr<IDXGISwapChain4> mSwapChain;
	D3D12_DESCRIPTOR_HEAP_DESC mRtvHeapDesc;  // RTVディスクリプタヒープの設定
	std::array<std::unique_ptr<RenderTarget>, 2> mBackBuffers;

	// フェンス
	ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	// 深度ステンシルバッファ
	std::unique_ptr<DepthBuffer> mDepthBuffer;

	// ルートシグネチャのエラーメッセージ
	ComPtr<ID3DBlob> mErrorBlob;  // シェーダーコンパイルエラーメッセージ

	// パイプライン状態バリア
	D3D12_RESOURCE_BARRIER mBarrierDesc;  // リソースバリアの設定

	// ビューポート
	std::unique_ptr<Viewport> mViewport;

	// シザー矩形
	std::unique_ptr<ScissorRectangle> mScissorRectangle;

	RenderWindow* mRenderWindow;

public:
	void Init();
	void PreDraw();
	void PostDraw();
	void PreIncrimentFenceValue() { ++mFenceValue; }

public:
	void TransitionBufferState(
		BufferResource* bufferResource,
		const D3D12_RESOURCE_STATES currentState,
		const D3D12_RESOURCE_STATES targetState);

private:
	// 初期化関連
	void DeviceInit();
	void CommandInit();
	void SwapChainInit();
	void FenceInit();
	void DepthBufferInit();
	void DescriptorHeapInit();
	void ShaderCompilerInit();
	void GraphicsPipelineInit();
	void ComputePipelineInit();

public:
	ID3D12Device* GetDevice() const;
	ID3D12GraphicsCommandList* GetCommandList() const;
	ID3D12CommandQueue* GetCommandQueue() const;
	ID3D12CommandAllocator* GetCommandAllocator() const;
	ID3D12Fence* GetFence() const;
	DepthBuffer* GetDepthBuffer() const;
	Viewport* GetViewport() const;
	UINT64 GetFenceValue() const;

private:
	RenderBase() {}
	~RenderBase() {}

	RenderBase(const RenderBase&) = delete;
	RenderBase& operator=(const RenderBase&) = delete;
	RenderBase(const RenderBase&&) = delete;
	RenderBase& operator=(const RenderBase&&) = delete;
public:
	static RenderBase* GetInstance()
	{
		static RenderBase* renderBase = new RenderBase;
		return renderBase;
	};

	static void Destroy()
	{
		delete GetInstance();
	}
};
