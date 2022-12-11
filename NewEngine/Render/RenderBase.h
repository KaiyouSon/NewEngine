#pragma once
#include "TextureBuffer.h"
#include "RenderWindow.h"
#include "ShaderObject.h"
#include "GraphicsPipeline.h"
#include "Util.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include <memory>

template<typename T> class Singleton;

class RenderBase : public Singleton<RenderBase>
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr <T>;
	static float clearColor[4];

public:
	void Init();
	void PreDraw();
	void PostDraw();
	void CreateSrv(Texture& texture, const D3D12_RESOURCE_DESC& textureResourceDesc);

private:
	// 初期化関連
	void DeviceInit();
	void CommandInit();
	void SwapChainInit();
	void FenceInit();
	void DepthBufferInit();
	void SrvInit();
	void ShaderCompilerInit();
	void RootSignatureInit();
	void GraphicsPipelineInit();

public:
	inline ID3D12Device* GetDevice() const { return device.Get(); }
	inline ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
	inline ID3D12DescriptorHeap* GetSrvDescHeap() const { return srvDescHeap.Get(); }
	inline ID3D12RootSignature* GetRootSignature() const { return rootSignature.Get(); }

	inline GraphicsPipeline* GetBasicPipeline()const { return basicPipeline.get(); }
	inline GraphicsPipeline* GetSpritePipeline()const { return spritePipeline.get(); }
	inline GraphicsPipeline* GetLinePipeline()const { return linePipeline.get(); }
	inline GraphicsPipeline* GetRenderTexturePipeline()const { return renderTexturePipeline.get(); }
	inline GraphicsPipeline* GetLoadModelPipeline()const { return loadModelPipeline.get(); }
	inline GraphicsPipeline* GetParticlePipeline()const { return particlePipeline.get(); }

private:
	friend Singleton<RenderBase>;

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
	ComPtr<ID3D12DescriptorHeap> rtvDescHeap;		 // rtv用デスクリプタヒープ
	std::vector<ComPtr<ID3D12Resource>> backBuffers; // バックバッファ

	// フェンス
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	// 深度バッファ
	ComPtr<ID3D12DescriptorHeap> dsvDescHeap;	// 深度バッファビュー用デスクリプタヒープ
	ComPtr<ID3D12Resource> depthBuffer;			// 深度バッファ

	// srv
	ComPtr<ID3D12DescriptorHeap> srvDescHeap;	// srv用デスクリプタヒープ
	UINT incrementIndex = 1;

	// シェーダコンパイラー
	std::unique_ptr<ShaderObject> basicShader;
	std::unique_ptr<ShaderObject> spriteShader;
	std::unique_ptr<ShaderObject> renderTextureShader;
	std::unique_ptr<ShaderObject> loadModelShader;
	std::unique_ptr<ShaderObject> particleShader;

	// グラフィックスパイプライン関連
	std::unique_ptr<GraphicsPipeline> basicPipeline;
	std::unique_ptr<GraphicsPipeline> spritePipeline;
	std::unique_ptr<GraphicsPipeline> linePipeline;
	std::unique_ptr<GraphicsPipeline> renderTexturePipeline;
	std::unique_ptr<GraphicsPipeline> loadModelPipeline;
	std::unique_ptr<GraphicsPipeline> particlePipeline;

	// ルートシグネチャー関連
	ComPtr <ID3DBlob> errorBlob;	// エラーオブジェクト
	ComPtr <ID3D12RootSignature> rootSignature;

	// 描画処理関連
	D3D12_RESOURCE_BARRIER barrierDesc{};	// リソースバリア
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;	// レンダーターゲットビューのハンドル

	RenderWindow* renderWindow;
};