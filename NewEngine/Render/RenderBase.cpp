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

	DeviceInit();			// �f�o�C�X�̏�����
	DescriptorHeapInit();	// �e�B�X�N���v�^�[�q�[�v�̏�����
	CommandInit();			// �R�}���h�֘A�̏�����
	SwapChainInit();		// �X���b�v�`�F���̏�����
	FenceInit();			// �t�F���X�̏�����
	DepthBufferInit();		// �[�x�o�b�t�@�̏�����
	ShaderCompilerInit();	// �V�F�[�_�[�R���p�C���[�̏�����
	RootSignatureInit();	// ���[�h�V�O�l�`���[�̏�����
	GraphicsPipelineInit();	// �O���t�B�b�N�X�p�C�v���C���̏�����
}
void RenderBase::PreDraw()
{
	//---------------------- ���\�[�X�o���A�̕ύX�R�}���h ----------------------//
	// �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();
	// �P�D���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc_.Transition.pResource = backBuffers_[bbIndex]->GetBuffer();	// �o�b�N�o�b�t�@���w��
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	// �\����Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	commandList_->ResourceBarrier(1, &barrierDesc_);

	//--------------------------- �`���w��R�}���h ---------------------------//
	// �Q�D�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = backBuffers_[bbIndex]->GetCpuHandle();

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthBuffer_->GetCpuHandle();
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// ��ʃN���A R G B A
	commandList_->ClearRenderTargetView(rtvHandle, sClearColor_, 0, nullptr);

	// �[�x�o�b�t�@�N���A
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// �r���[�|�[�g�̏���
	viewport_->SetViewport(
		{ 0,0 },
		{
			(float)renderWindow_->GetWindowSize().x,
			(float)renderWindow_->GetWindowSize().y
		});
	viewport_->Update();

	// �V�U�[��`�̏���
	scissorRectangle_->Update();

	//commandList_->SetDescriptorHeaps(1, srvDescHeap.GetAddressOf());


	// SRV�q�[�v�̐ݒ�R�}���h
	//auto temp = renderBase->GetSrvDescHeap();
}
void RenderBase::PostDraw()
{
	HRESULT result;

	//---------------------- ���\�[�X�o���A�̕��A�R�}���h ----------------------//
	// �T�D���\�[�X�o���A��߂�
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
	commandList_->ResourceBarrier(1, &barrierDesc_);

	//-------------------------- �R�}���h�̃t���b�V�� --------------------------//
	// ���߂̃N���[�Y
	result = commandList_->Close();
	assert(SUCCEEDED(result));
	// �R�}���h���X�g�̎��s
	ID3D12CommandList* commandList_s[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandList_s);

	// ��ʂɕ\������o�b�t�@���t���b�v�i���\�̓��ւ��j
	result = swapChain_->Present(1, 0);
	assert(SUCCEEDED(result));

	// �R�}���h�̎��s������҂�
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_.Get()->GetCompletedValue() != fenceVal_)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// �L���[���N���A
	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h���X�g�𒙂߂鏀��
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
	// RTV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = rtvDescHeap_->GetCPUDescriptorHandleForHeapStart();

	UINT descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	rtvCpuHandle.ptr += descriptorSize * rtvIncrementIndex_;

	renderTarget.SetCpuHandle(rtvCpuHandle);

	// �n���h���̎w���ʒu��RTV�쐬
	device_->CreateRenderTargetView(renderTarget.GetBuffer(), rtvDesc, rtvCpuHandle);

	rtvIncrementIndex_++;
}
void RenderBase::CreateDSV(DepthBuffer& depthBuffer_)
{
	// RTV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = dsvDescHeap_->GetCPUDescriptorHandleForHeapStart();

	UINT descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	dsvCpuHandle.ptr += descriptorSize * dsvIncrementIndex_;

	depthBuffer_.SetCpuHandle(dsvCpuHandle);

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	// �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	// �n���h���̎w���ʒu��RTV�쐬
	device_->CreateDepthStencilView(depthBuffer_.GetBuffer(), &dsvDesc, dsvCpuHandle);

	dsvIncrementIndex_++;
}

void RenderBase::DeviceInit()
{
	HRESULT result;

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}

	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);
		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i].Get();
			break;
		}
	}

	// �Ή����x���̔z��
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
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(device_.GetAddressOf()));
		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
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
	const size_t maxRTVCount = 64;	// RTV�̍ő��

	// RTV�p�f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// �����_�[�^�[�Q�b�g�r���[
	//rtvHeapDesc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
	rtvHeapDesc_.NumDescriptors = maxRTVCount; // ���\�̂Q��

	// RTV�p�f�X�N���v�^�q�[�v�̐���
	result = device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvDescHeap_));
	assert(SUCCEEDED(result));


	// --- DSV ------------------------------------------------------ //
	const size_t maxDSVCount = 64;	// DSV�̍ő��

	// DSV�p�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;		// �f�v�X�X�e���V���r���[
	//dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
	dsvHeapDesc.NumDescriptors = maxDSVCount;	// �[�x�r���[�͈��

	// DSV�p�f�X�N���v�^�q�[�v�̐���
	result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvDescHeap_));
	assert(SUCCEEDED(result));
}
void RenderBase::CommandInit()
{
	HRESULT result;

	// �R�}���h�A���P�[�^�𐶐�
	result = device_->CreateCommandAllocator
	(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_)
	);
	assert(SUCCEEDED(result));

	// �R�}���h���X�g�𐶐�
	result = device_->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_)
	);
	assert(SUCCEEDED(result));

	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueue_Desc{};
	//�R�}���h�L���[�𐶐�
	result = device_.Get()->CreateCommandQueue(&commandQueue_Desc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));
}
void RenderBase::SwapChainInit()
{
	HRESULT result;

	backBuffers_[0] = std::make_unique<RenderTarget>();
	backBuffers_[1] = std::make_unique<RenderTarget>();

	// ���\�[�X�̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChain_Desc{};
	swapChain_Desc.Width = (UINT)renderWindow_->GetWindowSize().x;
	swapChain_Desc.Height = (UINT)renderWindow_->GetWindowSize().y;
	swapChain_Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				 // �F���̏���
	swapChain_Desc.SampleDesc.Count = 1;								 // �}���`�T���v�����Ȃ�
	swapChain_Desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				 // �o�b�N�o�b�t�@�p
	swapChain_Desc.BufferCount = 2;									 // �o�b�t�@�����Q�ɐݒ�
	swapChain_Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		 // �t���b�v��͔j��
	swapChain_Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �X���b�v�`�F�[���̐���
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

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers_.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain_->GetBuffer((UINT)i, IID_PPV_ARGS(backBuffers_[i]->GetBufferAddress()));

		CreateRTV(*backBuffers_[i], &rtvDesc);
	}
}
void RenderBase::FenceInit()
{
	HRESULT result;

	// �t�F���X�̐���
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

	// Object3D�p�V�F�[�_�[
	ShaderObjectManager::Create("Object3D");
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->CompileVertexShader(path + "Object3DVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Object3D")->CompilePixelShader(path + "Object3DPS.hlsl", "main");

	// Fbx���f���p�V�F�[�_�[
	ShaderObjectManager::Create("FbxModel");
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->CompileVertexShader(path + "FbxModelVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("FbxModel")->CompilePixelShader(path + "FbxModelPS.hlsl", "main");

	// �X�v���C�g�p�V�F�[�_�[
	ShaderObjectManager::Create("Sprite");
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->CompileVertexShader(path + "SpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Sprite")->CompilePixelShader(path + "SpritePS.hlsl", "main");

	// �~�Q�[�W�X�v���C�g�p�V�F�[�_�[
	ShaderObjectManager::Create("CircleGaugeSprite");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompileVertexShader(path + "CircleGaugeSpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompilePixelShader(path + "CircleGaugeSpritePS.hlsl", "main");

	// �����_�[�e�N�X�`���[�̃V�F�[�_�[
	ShaderObjectManager::Create("RenderTexture");
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompileVertexShader(path + "RenderTextureVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompilePixelShader(path + "RenderTexturePS.hlsl", "main");

	// �V���G�b�g�p�V�F�[�_�[
	ShaderObjectManager::Create("Silhouette");
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->CompileVertexShader(path + "SilhouetteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Silhouette")->CompilePixelShader(path + "SilhouettePS.hlsl", "main");

	// �A�E�g���C��Object�p�V�F�[�_�[
	ShaderObjectManager::Create("Outline");
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->CompileVertexShader(path + "OutLineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Outline")->CompilePixelShader(path + "OutLinePS.hlsl", "main");

	// �g�D�[�������_�[�����O�p
	ShaderObjectManager::Create("ToonRender");
	ShaderObjectManager::GetShaderObject("ToonRender")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRender")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRender")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRender")->CompileVertexShader(path + "ToonRenderVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ToonRender")->CompilePixelShader(path + "ToonRenderPS.hlsl", "main");

	// ���C���p
	ShaderObjectManager::Create("Line");
	ShaderObjectManager::GetShaderObject("Line")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Line")->CompileVertexShader(path + "LineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Line")->CompilePixelShader(path + "LinePS.hlsl", "main");
}
void RenderBase::RootSignatureInit()
{
	// 3D�I�u�W�F�N�g�p
	object3DRootSignature_ = std::make_unique<RootSignature>();
	object3DRootSignature_->AddConstantBufferViewToRootRrameter(7);
	object3DRootSignature_->AddDescriptorRangeToRootPrameter(1);
	object3DRootSignature_->Create(1);

	// �X�v���C�g�p
	spriteRootSignature_ = std::make_unique<RootSignature>();
	spriteRootSignature_->AddConstantBufferViewToRootRrameter(3);
	spriteRootSignature_->AddDescriptorRangeToRootPrameter(1);
	spriteRootSignature_->Create(1);

	// �X�v���C�g�p
	renderTextureRootSignature_ = std::make_unique<RootSignature>();
	renderTextureRootSignature_->AddConstantBufferViewToRootRrameter(5);
	renderTextureRootSignature_->AddDescriptorRangeToRootPrameter(1);
	renderTextureRootSignature_->Create(2);
}
void RenderBase::GraphicsPipelineInit()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc1{};
	depthStencilDesc1.DepthEnable = true; // �[�x�e�X�g���s��
	depthStencilDesc1.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// �������݋���
	depthStencilDesc1.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// �������ق����̗p

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc2{};
	depthStencilDesc2.DepthEnable = false; // �[�x�e�X�g���s��Ȃ�

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc3{};
	depthStencilDesc3.DepthEnable = true; // �[�x�e�X�g���s��
	depthStencilDesc3.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// �������ݕs��
	depthStencilDesc3.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;	// �傫���ق����̗p

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc4{};
	depthStencilDesc4.DepthEnable = true; // �[�x�e�X�g���s��
	depthStencilDesc4.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// �������݋���
	depthStencilDesc4.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// �������ق����̗p

	// 3D�I�u�W�F�N�g�p
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Object3D"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Back,
		depthStencilDesc1,
		TopologyType::Triangle,
		2,
		"Object3D");

	// 3D�I�u�W�F�N�g�p
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("FbxModel"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Back,
		depthStencilDesc1,
		TopologyType::Triangle,
		1,
		"FbxModel");

	// �X�v���C�g�p
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Sprite"),
		spriteRootSignature_->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::Triangle,
		2,
		"Sprite");

	// �~�`�Q�[�W�X�v���C�g�p
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("CircleGaugeSprite"),
		spriteRootSignature_->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::Triangle,
		1,
		"CircleGaugeSprite");

	// �����_�[�e�N�X�`���p
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("RenderTexture"),
		renderTextureRootSignature_->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::Triangle,
		2,
		"RenderTexture");

	// �V���G�b�g�p
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Silhouette"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Back,
		depthStencilDesc3,
		TopologyType::Triangle,
		1,
		"Silhouette");

	// �A�E�g���C���p
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Outline"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Front,
		depthStencilDesc4,
		TopologyType::Triangle,
		1,
		"Outline");

	// �g�D�[�������_�����O�p
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("ToonRender"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::Back,
		depthStencilDesc1,
		TopologyType::Triangle,
		1,
		"ToonRendering");

	// ���C���p
	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Line"),
		object3DRootSignature_->GetRootSignature(),
		CullMode::None,
		depthStencilDesc1,
		TopologyType::Line,
		1,
		"Line");
}