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
	void CreateRTV(RenderTarget& renderTarget, const D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc);
	void CreateDSV(DepthBuffer& depthBuffer);

private:
	// �������֘A
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

	inline RootSignature* GetObject3DRootSignature() const { return object3DRootSignature.get(); }
	inline RootSignature* GetSpriteRootSignature() const { return spriteRootSignature.get(); }

	inline Viewport* GetViewport() const { return viewport.get(); }

	inline void PreIncrimentFenceValue() { ++fenceVal; }
	inline UINT64 GetFenceValue() { return fenceVal; }
private:
	//friend Singleton<RenderBase>;

	// �f�o�C�X�֘A
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;

	// �R�}���h�֘A
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;

	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> swapChain;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};		 // rtv�ݒ�\����
	std::array<std::unique_ptr<RenderTarget>, 2> backBuffers;

	// �t�F���X
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	// �[�x�o�b�t�@
	std::unique_ptr<DepthBuffer> depthBuffer;

	// �e�B�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> rtvDescHeap;		// rtv�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> dsvDescHeap;		// dsv�p�f�X�N���v�^�q�[�v
	UINT rtvIncrementIndex;
	UINT dsvIncrementIndex;

	// ���[�g�V�O�l�`���[�֘A
	ComPtr <ID3DBlob> errorBlob;	// �G���[�I�u�W�F�N�g
	std::unique_ptr<RootSignature> object3DRootSignature;
	std::unique_ptr<RootSignature> spriteRootSignature;

	// �`�揈���֘A
	D3D12_RESOURCE_BARRIER barrierDesc{};	// ���\�[�X�o���A
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