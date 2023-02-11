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
	static float clearColor[4];

public:
	void Init();
	void PreDraw();
	void PostDraw();
	void SetObject3DDrawCommand();
	void SetSpriteDrawCommand();
	void CreateSRV(Texture& texture, const D3D12_RESOURCE_DESC& textureResourceDesc);
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
	inline ID3D12Device* GetDevice() const { return device.Get(); }
	inline ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
	inline ID3D12CommandQueue* GetCommandQueue() const { return commandQueue.Get(); }
	inline ID3D12CommandAllocator* GetCommandAllocator() const { return commandAllocator.Get(); }
	inline ID3D12Fence* GetFence() const { return fence.Get(); }
	inline ID3D12DescriptorHeap* GetSrvDescHeap() const { return srvDescHeap.Get(); }

	inline RootSignature* GetObject3DRootSignature() const { return object3DRootSignature.get(); }
	inline RootSignature* GetSpriteRootSignature() const { return spriteRootSignature.get(); }

	inline GraphicsPipeline* GetLinePipeline() const { return linePipeline.get(); }

	inline Viewport* GetViewport() const { return viewport.get(); }

	inline void PreIncrimentFenceValue() { ++fenceVal; }
	inline UINT64 GetFenceValue() { return fenceVal; }
private:
	//friend Singleton<RenderBase>;

	// デバイス関連
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;

	// コマンド関連
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;

	// スワップチェーン
	ComPtr<IDXGISwapChain4> swapChain;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};		 // rtv設定構造体
	std::array<std::unique_ptr<RenderTarget>, 2> backBuffers;

	// フェンス
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	// 深度バッファ
	std::unique_ptr<DepthBuffer> depthBuffer;

	// ティスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> rtvDescHeap;		// rtv用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvDescHeap;		// srv用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvDescHeap;		// dsv用デスクリプタヒープ
	UINT rtvIncrementIndex;
	UINT srvIncrementIndex;
	UINT dsvIncrementIndex;

	// シェーダコンパイラー
	std::unique_ptr<ShaderObject> basicShader;
	std::unique_ptr<ShaderObject> spriteShader;
	std::unique_ptr<ShaderObject> circleGaugeSpriteShader;
	std::unique_ptr<ShaderObject> renderTextureShader;
	std::unique_ptr<ShaderObject> object3DShader;
	std::unique_ptr<ShaderObject> particleShader;
	std::unique_ptr<ShaderObject> silhouetteShader;
	std::unique_ptr<ShaderObject> outlineShader;
	std::unique_ptr<ShaderObject> toonRenderShader;

	// グラフィックスパイプライン関連
	std::unique_ptr<GraphicsPipeline> linePipeline;

	// ルートシグネチャー関連
	ComPtr <ID3DBlob> errorBlob;	// エラーオブジェクト
	std::unique_ptr<RootSignature> object3DRootSignature;
	std::unique_ptr<RootSignature> spriteRootSignature;

	// 描画処理関連
	D3D12_RESOURCE_BARRIER barrierDesc{};	// リソースバリア
	std::unique_ptr<Viewport> viewport;
	std::unique_ptr<ScissorRectangle> scissorRectangle;

	RenderWindow* renderWindow;

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