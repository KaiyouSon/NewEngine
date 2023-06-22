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

	// �f�o�C�X�֘A
	ComPtr<ID3D12Device> device_;
	ComPtr<IDXGIFactory7> dxgiFactory_;

	// �R�}���h�֘A
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<ID3D12CommandQueue> commandQueue_;

	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> swapChain_;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_;		 // rtv�ݒ�\����
	std::array<std::unique_ptr<RenderTarget>, 2> backBuffers_;

	// �t�F���X
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;

	// �[�x�o�b�t�@
	std::unique_ptr<DepthBuffer> depthBuffer_;

	// �e�B�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> rtvDescHeap_;		// rtv�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> dsvDescHeap_;		// dsv�p�f�X�N���v�^�q�[�v
	UINT rtvIncrementIndex_;
	UINT dsvIncrementIndex_;

	// ���[�g�V�O�l�`���[�֘A
	ComPtr <ID3DBlob> errorBlob_;	// �G���[�I�u�W�F�N�g
	std::unique_ptr<RootSignature> object3DRootSignature_;
	std::unique_ptr<RootSignature> spriteRootSignature_;
	std::unique_ptr<RootSignature> renderTextureRootSignature_;

	// �`�揈���֘A
	D3D12_RESOURCE_BARRIER barrierDesc_;	// ���\�[�X�o���A
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