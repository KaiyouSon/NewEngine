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

template<typename T> class Singleton;

class RenderBase// : public Singleton<RenderBase>
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr <T>;
	static float sClearColor[4];

private:
	// 繝・ヰ繧､繧ｹ髢｢騾｣
	ComPtr<ID3D12Device> mDevice;
	ComPtr<IDXGIFactory7> mDxgiFactory;

	// 繧ｳ繝槭Φ繝蛾未騾｣
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12CommandQueue> mCommandQueue;

	// 繧ｹ繝ｯ繝・・繝√ぉ繝ｼ繝ｳ
	ComPtr<IDXGISwapChain4> mSwapChain;
	D3D12_DESCRIPTOR_HEAP_DESC mRtvHeapDesc;		 // rtv險ｭ螳壽ｧ矩菴・
	std::array<std::unique_ptr<RenderTarget>, 2> mBackBuffers;

	// 繝輔ぉ繝ｳ繧ｹ
	ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	// 豺ｱ蠎ｦ繝舌ャ繝輔ぃ
	std::unique_ptr<DepthBuffer> mDepthBuffer;

	// 繝ｫ繝ｼ繝医す繧ｰ繝阪メ繝｣繝ｼ髢｢騾｣
	ComPtr<ID3DBlob> mErrorBlob;	// 繧ｨ繝ｩ繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝・

	// 謠冗判蜃ｦ逅・未騾｣
	D3D12_RESOURCE_BARRIER mBarrierDesc;	// 繝ｪ繧ｽ繝ｼ繧ｹ繝舌Μ繧｢
	std::unique_ptr<Viewport> mViewport;
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
	// 蛻晄悄蛹夜未騾｣
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
