#pragma once
#include "RenderWindow.h"
#include "ShaderObject.h"
#include "RootSignature.h"
#include "GraphicsPipelineManager.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "DepthBuffer.h"
#include "Viewport.h"
#include "ScissorRectangle.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include <memory>
#include <array>

template<typename T> class Singleton;

class RenderBase// : public Singleton<RenderBase>
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr <T>;
	static float sClearColor_[4];

public:
	void Init();
	void PreDraw();
	void PostDraw();
	void SetObject3DDrawCommand();
	void SetSpriteDrawCommand();
	void SetRenderTextureDrawCommand();
	void CreateRTV(RenderTarget& renderTarget, const D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc);
	void CreateDSV(DepthBuffer& depthBuffer);

private:
	// 初期化関連
	void DeviceInit();
	void CommandInit();
	void SwapChainInit();
	void FenceInit();
	void DepthBufferInit();
	void DescriptorHeapInit();
	void ShaderCompilerInit();
	void RootSignatureInit();
	void GraphicsPipelineInit();

public:
	inline ID3D12Device* GetDevice() const { return device_.Get(); }
	inline ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }
	inline ID3D12CommandQueue* GetCommandQueue() const { return commandQueue_.Get(); }
	inline ID3D12CommandAllocator* GetCommandAllocator() const { return commandAllocator_.Get(); }
	inline ID3D12Fence* GetFence() const { return fence_.Get(); }

	inline RootSignature* GetObject3DRootSignature() const { return object3DRootSignature_.get(); }
	inline RootSignature* GetSpriteRootSignature() const { return spriteRootSignature_.get(); }
	inline RootSignature* GetRenderTextureRootSignature() const { return renderTextureRootSignature_.get(); }

	inline DepthBuffer* GetDepthBuffer() const { return depthBuffer_.get(); }

	inline Viewport* GetViewport() const { return viewport_.get(); }

	inline void PreIncrimentFenceValue() { ++fenceVal_; }
	inline UINT64 GetFenceValue() { return fenceVal_; }
private:

	// デバイス関連
	ComPtr<ID3D12Device> device_;
	ComPtr<IDXGIFactory7> dxgiFactory_;

	// コマンド関連
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<ID3D12CommandQueue> commandQueue_;

	// スワップチェーン
	ComPtr<IDXGISwapChain4> swapChain_;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_;		 // rtv設定構造体
	std::array<std::unique_ptr<RenderTarget>, 2> backBuffers_;

	// フェンス
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;

	// 深度バッファ
	std::unique_ptr<DepthBuffer> depthBuffer_;

	// ティスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> rtvDescHeap_;		// rtv用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvDescHeap_;		// dsv用デスクリプタヒープ
	UINT rtvIncrementIndex_;
	UINT dsvIncrementIndex_;

	// ルートシグネチャー関連
	ComPtr <ID3DBlob> errorBlob_;	// エラーオブジェクト
	std::unique_ptr<RootSignature> object3DRootSignature_;
	std::unique_ptr<RootSignature> spriteRootSignature_;
	std::unique_ptr<RootSignature> renderTextureRootSignature_;

	// 描画処理関連
	D3D12_RESOURCE_BARRIER barrierDesc_;	// リソースバリア
	std::unique_ptr<Viewport> viewport_;
	std::unique_ptr<ScissorRectangle> scissorRectangle_;

	RenderWindow* renderWindow_;

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