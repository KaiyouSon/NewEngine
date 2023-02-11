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
	const size_t maxSRVCount = 2056;	// SRVの最大個数

	// SRV用デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
	srvHeapDesc.NumDescriptors = maxSRVCount;

	// SRV用デスクリプタヒープを生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvDescHeap));
	assert(SUCCEEDED(result));

}

void TextureManager::CreateSRV(Texture& texture)
{
	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = srvDescHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvDescHeap->GetGPUDescriptorHandleForHeapStart();

	UINT descriptorSize = RenderBase::GetInstance()->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	srvCpuHandle.ptr += descriptorSize * srvIncrementIndex;
	srvGpuHandle.ptr += descriptorSize * srvIncrementIndex;

	texture.SetCpuHandle(srvCpuHandle);
	texture.SetGpuHandle(srvGpuHandle);

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// srv設定構造体
	srvDesc.Format = texture.buffer->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2Dテクスチャ
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;	// 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = texture.buffer->GetDesc().MipLevels;

	// ハンドルの指す位置にシェーダーリソースビュー作成
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(texture.buffer.Get(), &srvDesc, srvCpuHandle);

	srvIncrementIndex++;
}
