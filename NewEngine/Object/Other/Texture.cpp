#include "Texture.h"
#include "RenderBase.h"
#pragma push
#pragma warning(disable:4023)
#include <DirectXTex.h>
#include <d3dx12.h>
#pragma pop
using namespace DirectX;

Texture::Texture() : result(HRESULT()), isMaterial(false)
{
}

Texture::Texture(const Color& color) : result(HRESULT()), isMaterial(false)
{
	// �q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	//textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//textureResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	//textureResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = 1; // ��
	textureResourceDesc.Height = 1; // ����
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���̃T�C�Y���Z�b�g
	size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// �e�N�X�`���o�b�t�@�̐���
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			//D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&buffer));
	assert(SUCCEEDED(result));

	buffer->SetName(L"TextureBuffer");

	RenderBase::GetInstance()->GetDevice()->
		GetCopyableFootprints(&textureResourceDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	UploadHeap(16);

	//Texture* textureMap;
	//textureMap = this;

	//// �e�N�X�`���o�b�t�@�̃}�b�s���O
	//result = buffer->Map(0, nullptr, (void**)&textureMap);	// �}�b�s���O
	//assert(SUCCEEDED(result));

	//Color tempColor;
	//tempColor = color / 255;
	//tempColor.a = color.a / 255;

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//result = buffer->WriteToSubresource(
	//	0,
	//	nullptr, // �S�̈�փR�s�[
	//	&tempColor,	// ���f�[�^�A�h���X
	//	sizeof(Color) * 1, // 1���C���T�C�Y
	//	sizeof(Color) * 1 // �S�T�C�Y
	//);

	RenderBase::GetInstance()->CreateSRV(*this, textureResourceDesc);
}

Texture::Texture(const std::string& filePath, const bool& isDirectoryPath) :
	result(HRESULT()), isMaterial(false)
{
	std::string path = isDirectoryPath ? filePath : "Application/Resources/Texture/" + filePath;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(path.begin(), path.end());

	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		assert(0 && "�e�N�X�`���[�̓ǂݍ��݂����s���܂���");
	}

	// �~�b�v�}�b�v����
	ScratchImage mipChain{};
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	// �q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // ��
	textureResourceDesc.Height = (UINT)metadata.height; // ����
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���̃T�C�Y���Z�b�g
	size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// �e�N�X�`���o�b�t�@�̐���
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&buffer));
	assert(SUCCEEDED(result));

	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// �S�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = buffer->WriteToSubresource(
			(UINT)i,
			nullptr,				// �S�̈�փR�s�[
			img->pixels,			// ���f�[�^�A�h���X
			(UINT)img->rowPitch,	// �P���C���T�C�Y
			(UINT)img->slicePitch	// �P���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	RenderBase::GetInstance()->CreateSRV(*this, textureResourceDesc);
}

void Texture::UploadHeap(const Vec2& size)
{
	// �q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(total_bytes);

	// �e�N�X�`���o�b�t�@�̐���
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&uploadBuffer));
	assert(SUCCEEDED(result));

	Color* ptr = nullptr;
	uploadBuffer->Map(0, nullptr, (void**)&ptr);
	assert(SUCCEEDED(result));

	Color tempColor;
	tempColor = Color::white / 255;
	tempColor.a = 1;

	//ptr = &tempColor;

	//////int a = sizeof(Color);

	memcpy(ptr, &tempColor, 16);

	//uploadBuffer->SetName(L"UploadBuffer");

	D3D12_TEXTURE_COPY_LOCATION destLocation;
	destLocation.pResource = buffer.Get();
	destLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	destLocation.SubresourceIndex = 0;

	D3D12_TEXTURE_COPY_LOCATION sourceLocation;
	sourceLocation.pResource = uploadBuffer.Get();
	sourceLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	sourceLocation.PlacedFootprint = footprint;

	ID3D12GraphicsCommandList* iCommandList = RenderBase::GetInstance()->GetCommandList();

	iCommandList->CopyTextureRegion(&destLocation, 0, 0, 0, &sourceLocation, nullptr);

	D3D12_RESOURCE_BARRIER  barrier1;
	//memset(&barrier1, 0, sizeof(barrie1));
	barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier1.Transition.pResource = buffer.Get();
	barrier1.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier1.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier1.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	iCommandList->Close();
	ID3D12CommandQueue* iCommandQueue = RenderBase::GetInstance()->GetCommandQueue();

	ID3D12CommandList* list[] = { iCommandList };
	iCommandQueue->ExecuteCommandLists(1, list);

	static int index = 0;

	// �R�}���h�̎��s������҂�
	iCommandQueue->Signal(RenderBase::GetInstance()->GetFence(), ++index);

	auto test = RenderBase::GetInstance()->GetFence()->GetCompletedValue();
	if (RenderBase::GetInstance()->GetFence()->GetCompletedValue() != index)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		RenderBase::GetInstance()->GetFence()->SetEventOnCompletion(index, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// �L���[���N���A
	result = RenderBase::GetInstance()->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result = iCommandList->Reset(RenderBase::GetInstance()->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));

	//memset(&barrier1, 0, sizeof(barrie1));

	//iCommandList->ResourceBarrier(1, &barrier1);
}
