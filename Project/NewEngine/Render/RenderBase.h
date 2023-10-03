#pragma once
#include "RenderWindow.h"
#include "ShaderObject.h"
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
	// �f�o�C�X�֘A
	ComPtr<ID3D12Device> mDevice;
	ComPtr<IDXGIFactory7> mDxgiFactory;

	// �R�}���h�֘A
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12CommandQueue> mCommandQueue;

	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> mSwapChain;
	D3D12_DESCRIPTOR_HEAP_DESC mRtvHeapDesc;		 // rtv�ݒ�\����
	std::array<std::unique_ptr<RenderTarget>, 2> mBackBuffers;

	// �t�F���X
	ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	// �[�x�o�b�t�@
	std::unique_ptr<DepthBuffer> mDepthBuffer;

	// �e�B�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> mRtvDescHeap;		// rtv�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> mDsvDescHeap;		// dsv�p�f�X�N���v�^�q�[�v
	uint32_t mRtvIncrementIndex;
	uint32_t mDsvIncrementIndex;

	// ���[�g�V�O�l�`���[�֘A
	ComPtr<ID3DBlob> mErrorBlob;	// �G���[�I�u�W�F�N�g

	// �`�揈���֘A
	D3D12_RESOURCE_BARRIER mBarrierDesc;	// ���\�[�X�o���A
	std::unique_ptr<Viewport> mViewport;
	std::unique_ptr<ScissorRectangle> mScissorRectangle;

	RenderWindow* mRenderWindow;

public:
	void Init();
	void PreDraw();
	void PostDraw();
	void PreIncrimentFenceValue() { ++mFenceValue; }

private:
	// �������֘A
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