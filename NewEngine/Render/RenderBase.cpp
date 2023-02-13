#include "RenderBase.h"
#include "RenderWindow.h"
#include "Viewport.h"
#include "ScissorRectangle.h"
#include "TextureManager.h"
#include <cassert>
#include <string>
#include <d3dcompiler.h>
#include <d3dx12.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
using namespace Microsoft::WRL;

float RenderBase::clearColor[4] = { 0.1f,0.25f,0.5f,0.0f };

void RenderBase::Init()
{
	renderWindow = RenderWindow::GetInstance().get();
	viewport = std::move(std::make_unique<Viewport>());
	scissorRectangle = std::move(std::make_unique<ScissorRectangle>());

	rtvIncrementIndex = 0;
	dsvIncrementIndex = 0;

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
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// �P�D���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc.Transition.pResource = backBuffers[bbIndex]->GetBuffer();	// �o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	// �\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	commandList->ResourceBarrier(1, &barrierDesc);

	//--------------------------- �`���w��R�}���h ---------------------------//
	// �Q�D�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = backBuffers[bbIndex]->GetCpuHandle();

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthBuffer->GetCpuHandle();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// ��ʃN���A R G B A
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// �[�x�o�b�t�@�N���A
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// �r���[�|�[�g�̏���
	viewport->SetViewport(
		{ 0,0 },
		{
			(float)renderWindow->GetWindowSize().x,
			(float)renderWindow->GetWindowSize().y
		});
	viewport->Update();

	// �V�U�[��`�̏���
	scissorRectangle->Update();

	//commandList->SetDescriptorHeaps(1, srvDescHeap.GetAddressOf());


	// SRV�q�[�v�̐ݒ�R�}���h
	//auto temp = renderBase->GetSrvDescHeap();
}
void RenderBase::PostDraw()
{
	HRESULT result;

	//---------------------- ���\�[�X�o���A�̕��A�R�}���h ----------------------//
	// �T�D���\�[�X�o���A��߂�
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
	commandList->ResourceBarrier(1, &barrierDesc);

	//-------------------------- �R�}���h�̃t���b�V�� --------------------------//
	// ���߂̃N���[�Y
	result = commandList->Close();
	assert(SUCCEEDED(result));
	// �R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);

	// ��ʂɕ\������o�b�t�@���t���b�v�i���\�̓��ւ��j
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	// �R�}���h�̎��s������҂�
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence.Get()->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// �L���[���N���A
	result = commandAllocator->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result = commandList.Get()->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}
void RenderBase::SetObject3DDrawCommand()
{
	commandList->SetGraphicsRootSignature(object3DRootSignature->GetRootSignature());
}
void RenderBase::SetSpriteDrawCommand()
{
	commandList->SetGraphicsRootSignature(spriteRootSignature->GetRootSignature());
}

void RenderBase::CreateRTV(RenderTarget& renderTarget, const D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	// RTV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();

	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	rtvCpuHandle.ptr += descriptorSize * rtvIncrementIndex;

	renderTarget.SetCpuHandle(rtvCpuHandle);

	// �n���h���̎w���ʒu��RTV�쐬
	device->CreateRenderTargetView(renderTarget.GetBuffer(), rtvDesc, rtvCpuHandle);

	rtvIncrementIndex++;
}
void RenderBase::CreateDSV(DepthBuffer& depthBuffer)
{
	// RTV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = dsvDescHeap->GetCPUDescriptorHandleForHeapStart();

	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	dsvCpuHandle.ptr += descriptorSize * dsvIncrementIndex;

	depthBuffer.SetCpuHandle(dsvCpuHandle);

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	// �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	// �n���h���̎w���ʒu��RTV�쐬
	device->CreateDepthStencilView(depthBuffer.GetBuffer(), &dsvDesc, dsvCpuHandle);

	dsvIncrementIndex++;
}

void RenderBase::DeviceInit()
{
	HRESULT result;

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
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
			IID_PPV_ARGS(device.GetAddressOf()));
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
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// �����_�[�^�[�Q�b�g�r���[
	//rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
	rtvHeapDesc.NumDescriptors = maxRTVCount; // ���\�̂Q��

	// RTV�p�f�X�N���v�^�q�[�v�̐���
	result = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescHeap));
	assert(SUCCEEDED(result));


	// --- DSV ------------------------------------------------------ //
	const size_t maxDSVCount = 64;	// DSV�̍ő��

	// DSV�p�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;		// �f�v�X�X�e���V���r���[
	//dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
	dsvHeapDesc.NumDescriptors = maxDSVCount;	// �[�x�r���[�͈��

	// DSV�p�f�X�N���v�^�q�[�v�̐���
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvDescHeap));
	assert(SUCCEEDED(result));
}
void RenderBase::CommandInit()
{
	HRESULT result;

	// �R�}���h�A���P�[�^�𐶐�
	result = device->CreateCommandAllocator
	(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator)
	);
	assert(SUCCEEDED(result));

	// �R�}���h���X�g�𐶐�
	result = device->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList)
	);
	assert(SUCCEEDED(result));

	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//�R�}���h�L���[�𐶐�
	result = device.Get()->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}
void RenderBase::SwapChainInit()
{
	HRESULT result;

	backBuffers[0] = std::move(std::make_unique<RenderTarget>());
	backBuffers[1] = std::move(std::make_unique<RenderTarget>());

	// ���\�[�X�̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = (UINT)renderWindow->GetWindowSize().x;
	swapChainDesc.Height = (UINT)renderWindow->GetWindowSize().y;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				 // �F���̏���
	swapChainDesc.SampleDesc.Count = 1;								 // �}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				 // �o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2;									 // �o�b�t�@�����Q�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		 // �t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �X���b�v�`�F�[���̐���
	ComPtr<IDXGISwapChain1> swapChain1;
	result = dxgiFactory->CreateSwapChainForHwnd
	(
		commandQueue.Get(),
		renderWindow->GetHwnd(),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain1
	);
	swapChain1.As(&swapChain);
	assert(SUCCEEDED(result));

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(backBuffers[i]->GetBufferAddress()));

		CreateRTV(*backBuffers[i], &rtvDesc);

		//D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = rtvSwapChainDescHeap->GetCPUDescriptorHandleForHeapStart();
		//rtvCpuHandle.ptr += i * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		//backBuffers[i]->SetCpuHandle(rtvCpuHandle);

		//device->CreateRenderTargetView(backBuffers[i]->GetBuffer(), &rtvDesc, rtvCpuHandle);
	}
}
void RenderBase::FenceInit()
{
	HRESULT result;

	// �t�F���X�̐���
	result = device->CreateFence(
		fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
}
void RenderBase::DepthBufferInit()
{
	HRESULT result;

	depthBuffer = std::move(std::make_unique<DepthBuffer>());
	depthBuffer->Create();
}
void RenderBase::ShaderCompilerInit()
{
	std::string path = "NewEngine/Shader/";

	// �X�v���C�g�p�V�F�[�_�[
	spriteShader = std::move(std::make_unique<ShaderObject>());
	spriteShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	spriteShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	spriteShader->CompileVertexShader(path + "SpriteVS.hlsl", "main");
	spriteShader->CompilePixelShader(path + "SpritePS.hlsl", "main");

	// �X�v���C�g�p�V�F�[�_�[
	circleGaugeSpriteShader = std::move(std::make_unique<ShaderObject>());
	circleGaugeSpriteShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	circleGaugeSpriteShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	circleGaugeSpriteShader->CompileVertexShader(path + "CircleGaugeSpriteVS.hlsl", "main");
	circleGaugeSpriteShader->CompilePixelShader(path + "CircleGaugeSpritePS.hlsl", "main");

	// �����_�[�e�N�X�`���[�̃V�F�[�_�[
	renderTextureShader = std::move(std::make_unique<ShaderObject>());
	renderTextureShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	renderTextureShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	renderTextureShader->CompileVertexShader(path + "RenderTextureVS.hlsl", "main");
	renderTextureShader->CompilePixelShader(path + "RenderTexturePS.hlsl", "main");

	// Object3D�p�V�F�[�_�[
	object3DShader = std::move(std::make_unique<ShaderObject>());
	object3DShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	object3DShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	object3DShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	object3DShader->CompileVertexShader(path + "Object3DVS.hlsl", "main");
	object3DShader->CompilePixelShader(path + "Object3DPS.hlsl", "main");

	// �V���G�b�g�p�V�F�[�_�[
	silhouetteShader = std::move(std::make_unique<ShaderObject>());
	silhouetteShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	silhouetteShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	silhouetteShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	silhouetteShader->CompileVertexShader(path + "SilhouetteVS.hlsl", "main");
	silhouetteShader->CompilePixelShader(path + "SilhouettePS.hlsl", "main");

	// �A�E�g���C��Object�p�V�F�[�_�[
	outlineShader = std::move(std::make_unique<ShaderObject>());
	outlineShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	outlineShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	outlineShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	outlineShader->CompileVertexShader(path + "OutLineVS.hlsl", "main");
	outlineShader->CompilePixelShader(path + "OutLinePS.hlsl", "main");

	// �g�D�[�������_�[�����O�p
	toonRenderShader = std::move(std::make_unique<ShaderObject>());
	toonRenderShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	toonRenderShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	toonRenderShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	toonRenderShader->CompileVertexShader(path + "ToonRenderVS.hlsl", "main");
	toonRenderShader->CompilePixelShader(path + "ToonRenderPS.hlsl", "main");

	// ���C���p
	lineShader = std::move(std::make_unique<ShaderObject>());
	lineShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	lineShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	lineShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	lineShader->CompileVertexShader(path + "LineVS.hlsl", "main");
	lineShader->CompilePixelShader(path + "LinePS.hlsl", "main");
}
void RenderBase::RootSignatureInit()
{
	// 3D�I�u�W�F�N�g�p
	object3DRootSignature = std::move(std::make_unique<RootSignature>());
	object3DRootSignature->AddConstantBufferViewToRootRrameter(7);
	object3DRootSignature->Create();

	// �X�v���C�g�p
	spriteRootSignature = std::move(std::make_unique<RootSignature>());
	spriteRootSignature->AddConstantBufferViewToRootRrameter(3);
	spriteRootSignature->Create();
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

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc4 = D3D12_DEPTH_STENCIL_DESC();
	depthStencilDesc4.DepthEnable = true; // �[�x�e�X�g���s��
	depthStencilDesc4.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// �������݋���
	depthStencilDesc4.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// �������ق����̗p

	// 3D�I�u�W�F�N�g�p
	GraphicsPipelineManager::Create(
		object3DShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullBack,
		depthStencilDesc1,
		TopologyType::TriangleTopology,
		"Object3D");

	// �X�v���C�g�p
	GraphicsPipelineManager::Create(
		spriteShader.get(),
		spriteRootSignature->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::TriangleTopology,
		"Sprite");

	// �~�`�Q�[�W�X�v���C�g�p
	GraphicsPipelineManager::Create(
		circleGaugeSpriteShader.get(),
		spriteRootSignature->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::TriangleTopology,
		"CircleGaugeSprite");

	// �����_�[�e�N�X�`���p
	GraphicsPipelineManager::Create(
		renderTextureShader.get(),
		spriteRootSignature->GetRootSignature(),
		CullMode::None,
		depthStencilDesc2,
		TopologyType::TriangleTopology,
		"RenderTexture");

	// �V���G�b�g�p
	GraphicsPipelineManager::Create(
		silhouetteShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullBack,
		depthStencilDesc3,
		TopologyType::TriangleTopology,
		"Silhouette");

	// �A�E�g���C���p
	GraphicsPipelineManager::Create(
		outlineShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullFront,
		depthStencilDesc4,
		TopologyType::TriangleTopology,
		"Outline");

	// �g�D�[�������_�����O�p
	GraphicsPipelineManager::Create(
		toonRenderShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullBack,
		depthStencilDesc1,
		TopologyType::TriangleTopology,
		"ToonRendering");

	// �R���C�_�[�p
	GraphicsPipelineManager::Create(
		lineShader.get(),
		object3DRootSignature->GetRootSignature(),
		CullMode::CullBack,
		depthStencilDesc1,
		TopologyType::TriangleTopology,
		"Collider");

	// Line�p
	linePipeline = std::move(std::make_unique<GraphicsPipeline>());
	linePipeline->SetShaderObject(object3DShader.get());
	linePipeline->SetCullMode(CullMode::None);
	linePipeline->SetDepthStencilDesc(depthStencilDesc1);
	linePipeline->SetTopologyType(TopologyType::LineTopology);
	linePipeline->SetRootSignature(object3DRootSignature->GetRootSignature());
	linePipeline->Create();
}