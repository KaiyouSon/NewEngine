#include "RenderBase.h"
#include "RenderWindow.h"
#include "Viewport.h"
#include "ScissorRectangle.h"
#include <cassert>
#include <string>
#include <d3dcompiler.h>
#include <d3dx12.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
using namespace Microsoft::WRL;
using namespace std;

unique_ptr<ScissorRectangle> scissorRectangle = move(make_unique<ScissorRectangle>());
unique_ptr<Viewport> viewport = move(make_unique<Viewport>());

float RenderBase::clearColor[4] = { 0.1f,0.25f,0.5f,1.0f };

void RenderBase::Init()
{
	renderWindow = RenderWindow::GetInstance().get();
	viewport = move(make_unique<Viewport>());

	DeviceInit();			// �f�o�C�X�̏�����
	CommandInit();			// �R�}���h�֘A�̏�����
	SwapChainInit();		// �X���b�v�`�F���̏�����
	FenceInit();			// �t�F���X�̏�����
	DepthBufferInit();		// �[�x�o�b�t�@�̏�����
	SrvInit();				// �V�F�[�_�[���\�[�X�r���[�֘A�̏�����
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
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();	// �o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	// �\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	commandList->ResourceBarrier(1, &barrierDesc);

	//--------------------------- �`���w��R�}���h ---------------------------//
	// �Q�D�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	rtvHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescHeap->GetCPUDescriptorHandleForHeapStart();
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
	//RenderBase::GetInstance()->PreIncreFenceVal());
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
void RenderBase::CreateSrv(Texture& texture, const D3D12_RESOURCE_DESC& textureResourceDesc)
{
	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = srvDescHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvDescHeap->GetGPUDescriptorHandleForHeapStart();

	UINT descriptorSize = RenderBase::GetInstance()->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	srvCpuHandle.ptr += descriptorSize * incrementIndex;
	srvGpuHandle.ptr += descriptorSize * incrementIndex;

	texture.SetCpuHandle(srvCpuHandle);
	texture.SetGpuHandle(srvGpuHandle);

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// srv�ݒ�\����
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(texture.buffer.Get(), &srvDesc, srvCpuHandle);

	incrementIndex++;
}

// �e������
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

	// �f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // ���\�̂Q��

	// �f�X�N���v�^�q�[�v�̐���
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescHeap));

	// �o�b�N�o�b�t�@
	backBuffers.resize(swapChainDesc.BufferCount);

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * device.Get()->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
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

	// ���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = (UINT)renderWindow->GetWindowSize().x;		// ��
	depthResourceDesc.Height = (UINT)renderWindow->GetWindowSize().y; // ����
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	// �[�x�l�f�t�H���g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	// �[�x�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	// �[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	// �[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	// �[�x�l�t�H�[�}�b�g

	// ���\�[�X�̐���
	result = device->CreateCommittedResource
	(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // �[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuffer)
	);
	assert(SUCCEEDED(result));

	// dsv�p�f�X�N���v�^�q�[�v�̍쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	// �[�x�r���[�͈��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvDescHeap));
	assert(SUCCEEDED(result));

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvViewDesc = {};
	dsvViewDesc.Format = DXGI_FORMAT_D32_FLOAT;	// �[�x�l�t�H�[�}�b�g
	dsvViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView
	(
		depthBuffer.Get(),
		&dsvViewDesc,
		dsvDescHeap->GetCPUDescriptorHandleForHeapStart()
	);

}
void RenderBase::SrvInit()
{

	HRESULT result;

	// SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// �ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvDescHeap));
	assert(SUCCEEDED(result));

}
void RenderBase::ShaderCompilerInit()
{

	// 3D�I�u�W�F�N�g�̃V�F�[�_�[
	basicShader = std::move(std::make_unique<ShaderObject>());
	basicShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	basicShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	basicShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	basicShader->CompileVertexShader("Shader/BasicVS.hlsl", "main");
	basicShader->CompilePixelShader("Shader/BasicPS.hlsl", "main");

	// �X�v���C�g�p�V�F�[�_�[
	spriteShader = std::move(std::make_unique<ShaderObject>());
	spriteShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	spriteShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	spriteShader->CompileVertexShader("Shader/SpriteVS.hlsl", "main");
	spriteShader->CompilePixelShader("Shader/SpritePS.hlsl", "main");

	// �����_�[�e�N�X�`���[�̃V�F�[�_�[
	renderTextureShader = std::move(std::make_unique<ShaderObject>());
	renderTextureShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	renderTextureShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	renderTextureShader->CompileVertexShader("Shader/RenderTextureVS.hlsl", "main");
	renderTextureShader->CompilePixelShader("Shader/RenderTexturePS.hlsl", "main");

	// ���[�h�������f���̃V�F�[�_�[
	loadModelShader = std::move(std::make_unique<ShaderObject>());
	loadModelShader->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	loadModelShader->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	loadModelShader->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	loadModelShader->CompileVertexShader("Shader/ObjectBasicVS.hlsl", "main");
	loadModelShader->CompilePixelShader("Shader/ObjectBasicPS.hlsl", "main");

}
void RenderBase::RootSignatureInit()
{
	HRESULT result;

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			// ��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂łP
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;		// �e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// �c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���s�J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// �S�Ẵ��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									// �~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												// �~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// �s�N�Z���V�F�[�_����̂ݎg�p�\

	// ���[�g�p�����[�^�[�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[7] = {};

	// �e�N�X�`�����W�X�^0�ԁiSRV�j
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ���
	rootParams[0].DescriptorTable.pDescriptorRanges = &descriptorRange;		  // �f�X�N���v�^�����W
	rootParams[0].DescriptorTable.NumDescriptorRanges = 1;					  // �f�X�N���v�^�����W��
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // �S�ẴV�F�[�_���猩����

	// �萔�o�b�t�@�i�g�����X�t�H�[���j
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
	rootParams[1].Descriptor.ShaderRegister = 0;					// �萔�o�b�t�@�ԍ�
	rootParams[1].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

	// �萔�o�b�t�@�i�}�e���A���j
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
	rootParams[2].Descriptor.ShaderRegister = 1;					// �萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

	// �萔�o�b�t�@�i�}�e���A���j
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
	rootParams[3].Descriptor.ShaderRegister = 2;					// �萔�o�b�t�@�ԍ�
	rootParams[3].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

	// �萔�o�b�t�@�i���s�����A�f�B���N�V���i�����C�g�j
	rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
	rootParams[4].Descriptor.ShaderRegister = 3;					// �萔�o�b�t�@�ԍ�
	rootParams[4].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

	// �萔�o�b�t�@�i�_�����j
	rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
	rootParams[5].Descriptor.ShaderRegister = 4;					// �萔�o�b�t�@�ԍ�
	rootParams[5].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

	// �萔�o�b�t�@�i�X�|�b�g���C�g�j
	rootParams[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
	rootParams[6].Descriptor.ShaderRegister = 5;					// �萔�o�b�t�@�ԍ�
	rootParams[6].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	// ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);		// ���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature
	(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	);
	assert(SUCCEEDED(result));

}
void RenderBase::GraphicsPipelineInit()
{
	// �O���t�B�b�N�X�p�C�v���C��3D�p
	basicPipeline = std::move(std::make_unique<GraphicsPipeline>());
	basicPipeline->SetShaderObject(basicShader.get());
	basicPipeline->SetCullMode(CullMode::CullBack);
	basicPipeline->SetisDepthEnable(true);
	basicPipeline->SetTopologyType(TopologyType::TriangleTopology);
	basicPipeline->SetRootSignature(rootSignature.Get());
	basicPipeline->Create();

	// �O���t�B�b�N�X�p�C�v���C��2D�p
	spritePipeline = std::move(std::make_unique<GraphicsPipeline>());
	spritePipeline->SetShaderObject(spriteShader.get());
	spritePipeline->SetCullMode(CullMode::None);
	spritePipeline->SetisDepthEnable(false);
	spritePipeline->SetTopologyType(TopologyType::TriangleTopology);
	spritePipeline->SetRootSignature(rootSignature.Get());
	spritePipeline->Create();

	// �O���t�B�b�N�X�p�C�v���C��Line�p
	linePipeline = std::move(std::make_unique<GraphicsPipeline>());
	linePipeline->SetShaderObject(basicShader.get());
	linePipeline->SetCullMode(CullMode::CullBack);
	linePipeline->SetisDepthEnable(true);
	linePipeline->SetTopologyType(TopologyType::LineTopology);
	linePipeline->SetRootSignature(rootSignature.Get());
	linePipeline->Create();

	// �����_�[�e�N�X�`���p
	renderTexturePipeline = std::move(std::make_unique<GraphicsPipeline>());
	renderTexturePipeline->SetShaderObject(renderTextureShader.get());
	renderTexturePipeline->SetCullMode(CullMode::CullBack);
	renderTexturePipeline->SetisDepthEnable(true);
	renderTexturePipeline->SetTopologyType(TopologyType::TriangleTopology);
	renderTexturePipeline->SetRootSignature(rootSignature.Get());
	renderTexturePipeline->Create();

	// �ǂݍ��݃I�u�W�F�N�g�p
	loadModelPipeline = std::move(std::make_unique<GraphicsPipeline>());
	loadModelPipeline->SetShaderObject(loadModelShader.get());
	loadModelPipeline->SetCullMode(CullMode::CullBack);
	loadModelPipeline->SetisDepthEnable(true);
	loadModelPipeline->SetTopologyType(TopologyType::TriangleTopology);
	loadModelPipeline->SetRootSignature(rootSignature.Get());
	loadModelPipeline->Create();

}