#include "TextureBuffer.h"
#include "RenderBase.h"
#include <cassert>
#include <d3dx12.h>
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

Vec4* TextureBuffer::imageData = new Vec4[imageDataCount];

TextureBuffer::~TextureBuffer()
{
	delete[] imageData;
}

unique_ptr<Texture> TextureBuffer::LoadTexture(const string filePath)
{
	HRESULT result;
	unique_ptr<Texture>texture = move(make_unique<Texture>());
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	wstring wfilePath(filePath.begin(), filePath.end());

	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (result != S_OK)
	{
		return texture;
	}
	//assert(SUCCEEDED(result));

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
	//texture->SetTextureSize(Vec2(textureResourceDesc.Width, textureResourceDesc.Height));

	// �e�N�X�`���o�b�t�@�̐���
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texture->buffer));
	assert(SUCCEEDED(result));

	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// �S�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texture->buffer->WriteToSubresource(
			(UINT)i,
			nullptr,				// �S�̈�փR�s�[
			img->pixels,			// ���f�[�^�A�h���X
			(UINT)img->rowPitch,	// �P���C���T�C�Y
			(UINT)img->slicePitch	// �P���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	RenderBase::GetInstance()->CreateSrv(*texture, textureResourceDesc);

	// �t�@�C���p�X

	return texture;
}
unique_ptr<Texture> TextureBuffer::GetDefaultTexture()
{
	HRESULT result;
	unique_ptr<Texture> texture = make_unique<Texture>();

	// �S�Ẵs�N�Z���̐F�̏�����
	for (size_t i = 0; i < imageDataCount; i++)
	{
		imageData[i].x = 1.0f;	// R
		imageData[i].y = 1.0f;	// G
		imageData[i].z = 1.0f;	// B
		imageData[i].w = 1.0f;	// A
	}

	// �q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = textureWidth; // ��
	textureResourceDesc.Height = textureWidth; // ����
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���̃T�C�Y���Z�b�g
	//texture->SetTextureSize(Vec2(textureResourceDesc.Width, textureResourceDesc.Height));

	// �e�N�X�`���o�b�t�@�̐���
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texture->buffer));
	assert(SUCCEEDED(result));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texture->buffer->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		imageData,	// ���f�[�^�A�h���X
		sizeof(Vec4) * textureWidth, // 1���C���T�C�Y
		sizeof(Vec4) * imageDataCount // �S�T�C�Y
	);

	RenderBase::GetInstance()->CreateSrv(*texture, textureResourceDesc);

	return texture;
}
unique_ptr<Texture> TextureBuffer::GetRenderTexture(const Vec2& size)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	HRESULT result;
	unique_ptr<Texture> texture = make_unique<Texture>();

	// �e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			(UINT64)size.x,
			(UINT)size.y,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	// �e�N�X�`���o�b�t�@�̐���
	CD3DX12_HEAP_PROPERTIES texHeapProperties =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE texClearValue =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, renderBase->clearColor);

	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&texHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&texClearValue,
			IID_PPV_ARGS(&texture->buffer));
	assert(SUCCEEDED(result));

	// �e�N�X�`����ԃN���A
	{
		// ��f��
		const UINT pixelCont = (UINT)size.x * (UINT)size.y;
		// �摜�P�s���̃f�[�^�T�C�Y
		const UINT rowPitch = sizeof(UINT) * (UINT)size.x;
		// �摜�S�̂̃f�[�^�T�C�Y
		const UINT depthPitch = rowPitch * (UINT)size.y;
		// �摜�C���[�W
		UINT* img = new UINT[pixelCont];
		for (UINT i = 0; i < pixelCont; i++) { img[i] = 0xff0000ff; }

		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texture->buffer->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	RenderBase::GetInstance()->CreateSrv(*texture, textureResourceDesc);

	return texture;
}

void TextureBuffer::UnLoadTexture(Texture* texture)
{
	delete texture;
}
