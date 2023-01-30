#include "Texture.h"
#include "RenderBase.h"
#pragma push
#pragma warning(disable:4023)
#include <DirectXTex.h>
#pragma pop
using namespace DirectX;

Texture::Texture() : result(HRESULT()), isMaterial(false)
{
}

Texture::Texture(const Color& color) : result(HRESULT()), isMaterial(false)
{
	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = 1; // 幅
	textureResourceDesc.Height = 1; // 高さ
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャのサイズをセット
	size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&buffer));
	assert(SUCCEEDED(result));

	Color tempColor;
	tempColor = color / 255;
	tempColor.a = color.a / 255;

	// テクスチャバッファにデータ転送
	result = buffer->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		&tempColor,	// 元データアドレス
		sizeof(Color) * 1, // 1ラインサイズ
		sizeof(Color) * 1 // 全サイズ
	);

	RenderBase::GetInstance()->CreateSRV(*this, textureResourceDesc);
}

Texture::Texture(const std::string& filePath, const bool& isDirectoryPath) :
	result(HRESULT()), isMaterial(false)
{
	std::string path = isDirectoryPath ? filePath : "Application/Resources/Texture/" + filePath;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(path.begin(), path.end());

	// WICテクスチャのロード
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		assert(0 && "テクスチャーの読み込みが失敗しました");
	}

	// ミップマップ生成
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

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // 幅
	textureResourceDesc.Height = (UINT)metadata.height; // 高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャのサイズをセット
	size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&buffer));
	assert(SUCCEEDED(result));

	// 全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// 全ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = buffer->WriteToSubresource(
			(UINT)i,
			nullptr,				// 全領域へコピー
			img->pixels,			// 元データアドレス
			(UINT)img->rowPitch,	// １ラインサイズ
			(UINT)img->slicePitch	// １枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	RenderBase::GetInstance()->CreateSRV(*this, textureResourceDesc);
}