#include "TextureManager.h"
#include "RenderBase.h"
#include <DirectXTex.h>
using namespace DirectX;

UINT TextureManager::srvIncrementIndex = 1;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> TextureManager::srvDescHeap;
std::map<std::string, std::unique_ptr<Texture>> TextureManager::textureMap;

Texture* TextureManager::GetTexture(const std::string& textureTag)
{
	return textureMap[textureTag].get();
}

Texture* TextureManager::CreateTexture(const Color& color, const std::string& textureTag)
{
	std::unique_ptr<Texture> tex = std::make_unique<Texture>(color);
	textureMap.insert(std::make_pair(textureTag, std::move(tex)));

	return textureMap[textureTag].get();
}

Texture* TextureManager::LoadTexture(const std::string& filePath, const std::string& textureTag)
{
	std::unique_ptr<Texture> tex;
	tex.reset(new Texture(filePath));
	textureMap.insert(std::make_pair(textureTag, std::move(tex)));

	return textureMap[textureTag].get();
}

void TextureManager::CreateDescriptorHeap()
{
	HRESULT result;

	// --- SRV ------------------------------------------------------ //
	const size_t maxSRVCount = 2056;	// SRV�̍ő��

	// SRV�p�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = maxSRVCount;

	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = RenderBase::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvDescHeap));
	assert(SUCCEEDED(result));

}

void TextureManager::CreateSRV(Texture& texture)
{
	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = srvDescHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvDescHeap->GetGPUDescriptorHandleForHeapStart();

	UINT descriptorSize = RenderBase::GetInstance()->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	srvCpuHandle.ptr += descriptorSize * srvIncrementIndex;
	srvGpuHandle.ptr += descriptorSize * srvIncrementIndex;

	texture.SetCpuHandle(srvCpuHandle);
	texture.SetGpuHandle(srvGpuHandle);

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// srv�ݒ�\����
	srvDesc.Format = texture.buffer->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2D�e�N�X�`��
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;	// 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = texture.buffer->GetDesc().MipLevels;

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(texture.buffer.Get(), &srvDesc, srvCpuHandle);

	srvIncrementIndex++;
}
