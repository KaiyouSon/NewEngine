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

float RenderBase::sClearColor[4] = { 0.1f,0.25f,0.5f,0.0f };

void RenderBase::Init()
{
	// �f�o�b�O���̂ݎ��s
	ProcessAtDebugBulid([]()
		{
			//�f�o�b�O���C���[���I����
			ComPtr<ID3D12Debug1> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
			{
				debugController->EnableDebugLayer();
				debugController->SetEnableGPUBasedValidation(false);
			}
		});

	DeviceInit();	// �f�o�C�X�̏�����

	// �f�o�b�O���̂ݎ��s
	ProcessAtDebugBulid([]()
		{
			ComPtr<ID3D12InfoQueue> infoQueue;
			if (SUCCEEDED(RenderBase::GetInstance()->
				GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue))))
			{
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	// ��΂��G���[�ꎞ�Ɏ~�܂�
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		// �G���[���Ɏ~�܂�
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	// ���[�j���O���Ɏ~�܂�
			}

			//�@�}������G���[
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
			};

			//�@�}�������\�����x��
			D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
			D3D12_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = _countof(denyIds);
			filter.DenyList.pIDList = denyIds;
			filter.DenyList.NumSeverities = _countof(severities);
			filter.DenyList.pSeverityList = severities;
			//�@�w�肵���G���[�̕\����}������
			infoQueue->PushStorageFilter(&filter);
		});

	mRenderWindow = RenderWindow::GetInstance().get();
	mViewport = std::make_unique<Viewport>();
	mScissorRectangle = std::make_unique<ScissorRectangle>();

	mFenceValue = 0;

	DescriptorHeapInit();	// �e�B�X�N���v�^�[�q�[�v�̏�����
	CommandInit();			// �R�}���h�֘A�̏�����
	SwapChainInit();		// �X���b�v�`�F���̏�����
	FenceInit();			// �t�F���X�̏�����
	DepthBufferInit();		// �[�x�o�b�t�@�̏�����
	ShaderCompilerInit();	// �V�F�[�_�[�R���p�C���[�̏�����
	GraphicsPipelineInit();	// �O���t�B�b�N�X�p�C�v���C���̏�����
	ComputePipelineInit();	// �R���s���[�g�p�C�v���C���̏�����
}
void RenderBase::PreDraw()
{
	//---------------------- ���\�[�X�o���A�̕ύX�R�}���h ----------------------//
	// �o�b�N�o�b�t�@�̔ԍ����擾
	UINT bbIndex = mSwapChain->GetCurrentBackBufferIndex();
	// �P�D���\�[�X�o���A�ŏ������݉\�ɕύX
	mBarrierDesc.Transition.pResource = mBackBuffers[bbIndex]->GetBufferResource()->buffer.Get();	// �o�b�N�o�b�t�@���w��
	mBarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	// �\����Ԃ���
	mBarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	mCommandList->ResourceBarrier(1, &mBarrierDesc);

	//--------------------------- �`���w��R�}���h ---------------------------//
	// �Q�D�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mBackBuffers[bbIndex]->GetBufferResource()->rtvHandle.cpu;

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDepthBuffer->GetBufferResource()->dsvHandle.cpu;
	mCommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// ��ʃN���A R G B A
	mCommandList->ClearRenderTargetView(rtvHandle, sClearColor, 0, nullptr);

	// �[�x�o�b�t�@�N���A
	mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// �r���[�|�[�g�̏���
	mViewport->SetViewport(
		{ 0,0 },
		{
			(float)mRenderWindow->GetWindowSize().x,
			(float)mRenderWindow->GetWindowSize().y
		});
	mViewport->Update();

	// �V�U�[��`�̏���
	mScissorRectangle->Update();
}
void RenderBase::PostDraw()
{
	HRESULT result;

	//---------------------- ���\�[�X�o���A�̕��A�R�}���h ----------------------//
	// �T�D���\�[�X�o���A��߂�
	mBarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
	mBarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
	mCommandList->ResourceBarrier(1, &mBarrierDesc);

	//-------------------------- �R�}���h�̃t���b�V�� --------------------------//
	// ���߂̃N���[�Y
	result = mCommandList->Close();
	assert(SUCCEEDED(result));
	// �R�}���h���X�g�̎��s
	ID3D12CommandList* mCommandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(1, mCommandLists);

	// ��ʂɕ\������o�b�t�@���t���b�v�i���\�̓��ւ��j
	result = mSwapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	// �R�}���h�̎��s������҂�
	mCommandQueue->Signal(mFence.Get(), ++mFenceValue);
	if (mFence.Get()->GetCompletedValue() != mFenceValue)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// �L���[���N���A
	result = mCommandAllocator->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result = mCommandList.Get()->Reset(mCommandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void RenderBase::TransitionBufferState(
	BufferResource* bufferResource,
	const D3D12_RESOURCE_STATES currentState,
	const D3D12_RESOURCE_STATES targetState)
{
	CD3DX12_RESOURCE_BARRIER barrier =
		CD3DX12_RESOURCE_BARRIER::Transition(
			bufferResource->buffer.Get(),
			currentState,
			targetState,
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
	mCommandList->ResourceBarrier(1, &barrier);
}

// --- �������֘A ------------------------------------------------------------ //
void RenderBase::DeviceInit()
{
	HRESULT result;

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&mDxgiFactory));
	assert(SUCCEEDED(result));

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		mDxgiFactory->EnumAdapterByGpuPreference(i,
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
			IID_PPV_ARGS(mDevice.GetAddressOf()));
		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

}
void RenderBase::DescriptorHeapInit()
{
	DescriptorHeapSetting setting;
	// SRV�i�[�p
	setting.maxSize = 2048;
	setting.startIndex = 1;
	setting.heapType = DescriptorHeapSetting::CBV_SRV_UAV;
	DescriptorHeapManager::Create(setting, "SRV");

	// RTV�i�[�p
	setting.maxSize = 64;
	setting.heapType = DescriptorHeapSetting::RTV;
	DescriptorHeapManager::Create(setting, "RTV");

	// DSV�i�[�p
	setting.maxSize = 64;
	setting.heapType = DescriptorHeapSetting::DSV;
	DescriptorHeapManager::Create(setting, "DSV");

	// SRV_UAV�i�[�p
	setting.maxSize = 512;
	setting.heapType = DescriptorHeapSetting::CBV_SRV_UAV;
	DescriptorHeapManager::Create(setting, "SRV_UAV");
}
void RenderBase::CommandInit()
{
	HRESULT result;

	// �R�}���h�A���P�[�^�𐶐�
	result = mDevice->CreateCommandAllocator
	(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&mCommandAllocator)
	);
	assert(SUCCEEDED(result));

	// �R�}���h���X�g�𐶐�
	result = mDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCommandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&mCommandList)
	);
	assert(SUCCEEDED(result));

	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC mCommandQueueDesc{};
	//�R�}���h�L���[�𐶐�
	result = mDevice.Get()->CreateCommandQueue(&mCommandQueueDesc, IID_PPV_ARGS(&mCommandQueue));
	assert(SUCCEEDED(result));
}
void RenderBase::SwapChainInit()
{
	HRESULT result;

	mBackBuffers[0] = std::make_unique<RenderTarget>();
	mBackBuffers[1] = std::make_unique<RenderTarget>();

	// ���\�[�X�̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 mSwapChainDesc{};
	mSwapChainDesc.Width = (UINT)mRenderWindow->GetWindowSize().x;
	mSwapChainDesc.Height = (UINT)mRenderWindow->GetWindowSize().y;
	mSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				 // �F���̏���
	mSwapChainDesc.SampleDesc.Count = 1;							 // �}���`�T���v�����Ȃ�
	mSwapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;			 // �o�b�N�o�b�t�@�p
	mSwapChainDesc.BufferCount = 2;									 // �o�b�t�@�����Q�ɐݒ�
	mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		 // �t���b�v��͔j��
	mSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �X���b�v�`�F�[���̐���
	ComPtr<IDXGISwapChain1> mSwapChain1;
	result = mDxgiFactory->CreateSwapChainForHwnd
	(
		mCommandQueue.Get(),
		mRenderWindow->GetHwnd(),
		&mSwapChainDesc,
		nullptr,
		nullptr,
		&mSwapChain1
	);
	mSwapChain1.As(&mSwapChain);
	assert(SUCCEEDED(result));

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < mBackBuffers.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		mSwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(mBackBuffers[i]->GetBufferResource()->buffer.GetAddressOf()));

		// RTV�쐬
		DescriptorHeapManager::GetDescriptorHeap("RTV")->CreateRTV(mBackBuffers[i]->GetBufferResource());
	}
}
void RenderBase::FenceInit()
{
	HRESULT result;

	// �t�F���X�̐���
	result = mDevice->CreateFence(
		mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
}
void RenderBase::DepthBufferInit()
{
	mDepthBuffer = std::make_unique<DepthBuffer>();
	mDepthBuffer->Create();
	DescriptorHeapManager::GetDescriptorHeap("DSV")->CreateDSV(mDepthBuffer->GetBufferResource());
}
void RenderBase::ShaderCompilerInit()
{
	std::string path1 = "NewEngine/Shader/";
	std::string path2 = "Application/Shader/";

	// Object3D�p�V�F�[�_�[
	ShaderObjectManager::Create("Object3D");
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->CompileVertexShader(path1 + "Object3DVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Object3D")->CompilePixelShader(path1 + "Object3DPS.hlsl", "main");

	// Fbx���f���p�V�F�[�_�[
	ShaderObjectManager::Create("FbxModel");
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->CompileVertexShader(path1 + "FbxModelVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("FbxModel")->CompilePixelShader(path1 + "FbxModelPS.hlsl", "main");

	// �X�v���C�g�p�V�F�[�_�[
	ShaderObjectManager::Create("Sprite");
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->CompileVertexShader(path1 + "SpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Sprite")->CompilePixelShader(path1 + "SpritePS.hlsl", "main");

	// �~�Q�[�W�X�v���C�g�p�V�F�[�_�[
	ShaderObjectManager::Create("CircleGaugeSprite");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompileVertexShader(path1 + "CircleGaugeSpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompilePixelShader(path1 + "CircleGaugeSpritePS.hlsl", "main");

	// �����_�[�e�N�X�`���[�̃V�F�[�_�[
	ShaderObjectManager::Create("RenderTexture");
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompileVertexShader(path1 + "RenderTextureVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompilePixelShader(path1 + "RenderTexturePS.hlsl", "main");

	// �V���G�b�g�p�V�F�[�_�[
	ShaderObjectManager::Create("Silhouette");
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->CompileVertexShader(path1 + "SilhouetteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Silhouette")->CompilePixelShader(path1 + "SilhouettePS.hlsl", "main");

	// �A�E�g���C��Object�p�V�F�[�_�[
	ShaderObjectManager::Create("Outline");
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->CompileVertexShader(path1 + "OutLineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Outline")->CompilePixelShader(path1 + "OutLinePS.hlsl", "main");

	// �g�D�[�������_�[�����O�p
	ShaderObjectManager::Create("ToonRendering");
	ShaderObjectManager::GetShaderObject("ToonRendering")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRendering")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRendering")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRendering")->CompileVertexShader(path1 + "ToonRenderVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ToonRendering")->CompilePixelShader(path1 + "ToonRenderPS.hlsl", "main");

	// ���C���p
	ShaderObjectManager::Create("Line");
	ShaderObjectManager::GetShaderObject("Line")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Line")->CompileVertexShader(path1 + "LineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Line")->CompilePixelShader(path1 + "LinePS.hlsl", "main");

	// �G�~�b�^�[�p
	ShaderObjectManager::Create("Emitter");
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);	// ���W
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);		// �X�P�[��
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 1);		// ��]
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 2);		// �P���x
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);	// �F
	ShaderObjectManager::GetShaderObject("Emitter")->CompileComputeShader(path1 + "EmitterCS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Emitter")->CompileVertexShader(path1 + "EmitterVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Emitter")->CompileGeometryShader(path1 + "EmitterGS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Emitter")->CompilePixelShader(path1 + "EmitterPS.hlsl", "main");

	// GPU�G�~�b�^�[�p
	ShaderObjectManager::Create("GPUEmitter");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompileComputeShader(path2 + "RespawnPointEffectCS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompileVertexShader(path2 + "RespawnPointEffectVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompileGeometryShader(path1 + "EmitterGS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompilePixelShader(path1 + "EmitterPS.hlsl", "main");

	// ColliderObject�p�V�F�[�_�[
	ShaderObjectManager::Create("ColliderObject");
	ShaderObjectManager::GetShaderObject("ColliderObject")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ColliderObject")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ColliderObject")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ColliderObject")->CompileVertexShader(path1 + "ColliderObjectVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ColliderObject")->CompilePixelShader(path1 + "ColliderObjectPS.hlsl", "main");
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

	GraphicsPipelineSetting setting;

	// 3D�I�u�W�F�N�g�p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Object3D");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 8;
	setting.rootSignatureSetting.maxSrvDescritorRange = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "Object3D");

	// FBX���f���p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("FbxModel");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "FbxModel");

	// �X�v���C�g�p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Sprite");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Sprite");

	// �~�`�Q�[�W�X�v���C�g�p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("CircleGaugeSprite");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "CircleGaugeSprite");

	// �����_�[�e�N�X�`���p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("RenderTexture");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "RenderTexture");

	// �V���G�b�g�p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Silhouette");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc3;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Silhouette");

	// �A�E�g���C���p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Outline");
	setting.cullMode = CullMode::Front;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc4;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Outline");

	// �g�D�[�������_�����O�p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ToonRendering");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc4;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "ToonRendering");

	// ���C���p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Line");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::LineStrip;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "Line");

	// �G�~�b�^�[�p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Emitter");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Emitter");

	// GPU�G�~�b�^�[�p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("GPUEmitter");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "GPUEmitter");

	// ColliderObject�p
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ColliderObject");
	setting.cullMode = CullMode::None;
	setting.fillMode = GraphicsPipelineSetting::Wireframe;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "ColliderObject");
}
void RenderBase::ComputePipelineInit()
{
	ComputePipelineSetting setting;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("GPUEmitter");
	setting.rootSignatureSetting.maxCbvRootParameter = 0;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "GPUEmitter");
}

// --- �Q�b�^�[ -------------------------------------------------------------- //
ID3D12Device* RenderBase::GetDevice() const
{
	return mDevice.Get();
}
ID3D12GraphicsCommandList* RenderBase::GetCommandList() const
{
	return mCommandList.Get();
}
ID3D12CommandQueue* RenderBase::GetCommandQueue() const
{
	return mCommandQueue.Get();
}
ID3D12CommandAllocator* RenderBase::GetCommandAllocator() const
{
	return mCommandAllocator.Get();
}
ID3D12Fence* RenderBase::GetFence() const
{
	return mFence.Get();
}
DepthBuffer* RenderBase::GetDepthBuffer() const
{
	return mDepthBuffer.get();
}
Viewport* RenderBase::GetViewport() const
{
	return mViewport.get();
}
UINT64 RenderBase::GetFenceValue() const
{
	return mFenceValue;
}