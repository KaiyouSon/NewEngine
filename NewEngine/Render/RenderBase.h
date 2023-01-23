#pragma once
#include "RenderWindow.h"
#include "ShaderObject.h"
#include "RootSignature.h"
#include "GraphicsPipeline.h"
#include "Texture.h"
#include "Util.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include <memory>

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
	void CreateSrv(Texture& texture, const D3D12_RESOURCE_DESC& textureResourceDesc);

private:
	// �������֘A
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

	inline RootSignature* GetObject3DRootSignature() const { return object3DRootSignature.get(); }
	inline RootSignature* GetSpriteRootSignature() const { return spriteRootSignature.get(); }

	inline GraphicsPipeline* GetBasicPipeline()const { return basicPipeline.get(); }
	inline GraphicsPipeline* GetSpritePipeline()const { return spritePipeline.get(); }
	inline GraphicsPipeline* GetLinePipeline()const { return linePipeline.get(); }
	inline GraphicsPipeline* GetRenderTexturePipeline()const { return renderTexturePipeline.get(); }
	inline GraphicsPipeline* GetObject3DPipeline()const { return object3DPipeline.get(); }
	inline GraphicsPipeline* GetSilhouettePipeline()const { return silhouettePipeline.get(); }
	inline GraphicsPipeline* GetOutLinePipeline()const { return outlinePipeline.get(); }

	inline Viewport* GetViewport() const { return viewport.get(); }
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
	ComPtr<ID3D12DescriptorHeap> rtvDescHeap;		 // rtv�p�f�X�N���v�^�q�[�v
	std::vector<ComPtr<ID3D12Resource>> backBuffers; // �o�b�N�o�b�t�@

	// �t�F���X
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	// �[�x�o�b�t�@
	ComPtr<ID3D12DescriptorHeap> dsvDescHeap;	// �[�x�o�b�t�@�r���[�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12Resource> depthBuffer;			// �[�x�o�b�t�@

	// srv
	ComPtr<ID3D12DescriptorHeap> srvDescHeap;	// srv�p�f�X�N���v�^�q�[�v
	UINT incrementIndex = 1;

	// �V�F�[�_�R���p�C���[
	std::unique_ptr<ShaderObject> basicShader;
	std::unique_ptr<ShaderObject> spriteShader;
	std::unique_ptr<ShaderObject> renderTextureShader;
	std::unique_ptr<ShaderObject> object3DShader;
	std::unique_ptr<ShaderObject> particleShader;
	std::unique_ptr<ShaderObject> silhouetteShader;
	std::unique_ptr<ShaderObject> outlineShader;

	// �O���t�B�b�N�X�p�C�v���C���֘A
	std::unique_ptr<GraphicsPipeline> basicPipeline;
	std::unique_ptr<GraphicsPipeline> spritePipeline;
	std::unique_ptr<GraphicsPipeline> linePipeline;
	std::unique_ptr<GraphicsPipeline> renderTexturePipeline;
	std::unique_ptr<GraphicsPipeline> object3DPipeline;
	std::unique_ptr<GraphicsPipeline> silhouettePipeline;
	std::unique_ptr<GraphicsPipeline> outlinePipeline;

	// ���[�g�V�O�l�`���[�֘A
	ComPtr <ID3DBlob> errorBlob;	// �G���[�I�u�W�F�N�g
	std::unique_ptr<RootSignature> object3DRootSignature;
	std::unique_ptr<RootSignature> spriteRootSignature;

	// �`�揈���֘A
	D3D12_RESOURCE_BARRIER barrierDesc{};	// ���\�[�X�o���A
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;	// �����_�[�^�[�Q�b�g�r���[�̃n���h��

	RenderWindow* renderWindow;

	std::unique_ptr<Viewport> viewport;

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