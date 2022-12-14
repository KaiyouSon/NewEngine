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

	// WICテクスチャのロード
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (result != S_OK)
	{
		return texture;
	}
	//assert(SUCCEEDED(result));

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
	//texture->SetTextureSize(Vec2(textureResourceDesc.Width, textureResourceDesc.Height));

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texture->buffer));
	assert(SUCCEEDED(result));

	// 全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// 全ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = texture->buffer->WriteToSubresource(
			(UINT)i,
			nullptr,				// 全領域へコピー
			img->pixels,			// 元データアドレス
			(UINT)img->rowPitch,	// １ラインサイズ
			(UINT)img->slicePitch	// １枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	RenderBase::GetInstance()->CreateSrv(*texture, textureResourceDesc);

	// ファイルパス

	return texture;
}
unique_ptr<Texture> TextureBuffer::GetDefaultTexture()
{
	HRESULT result;
	unique_ptr<Texture> texture = make_unique<Texture>();

	// 全てのピクセルの色の初期化
	for (size_t i = 0; i < imageDataCount; i++)
	{
		imageData[i].x = 1.0f;	// R
		imageData[i].y = 1.0f;	// G
		imageData[i].z = 1.0f;	// B
		imageData[i].w = 1.0f;	// A
	}

	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = textureWidth; // 幅
	textureResourceDesc.Height = textureWidth; // 高さ
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャのサイズをセット
	//texture->SetTextureSize(Vec2(textureResourceDesc.Width, textureResourceDesc.Height));

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texture->buffer));
	assert(SUCCEEDED(result));

	// テクスチャバッファにデータ転送
	result = texture->buffer->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		imageData,	// 元データアドレス
		sizeof(Vec4) * textureWidth, // 1ラインサイズ
		sizeof(Vec4) * imageDataCount // 全サイズ
	);

	RenderBase::GetInstance()->CreateSrv(*texture, textureResourceDesc);

	return texture;
}
unique_ptr<Texture> TextureBuffer::GetRenderTexture(const Vec2& size)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	HRESULT result;
	unique_ptr<Texture> texture = make_unique<Texture>();

	// テクスチャリソース設定
	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			(UINT64)size.x,
			(UINT)size.y,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	// テクスチャバッファの生成
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

	// テクスチャを赤クリア
	{
		// 画素数
		const UINT pixelCont = (UINT)size.x * (UINT)size.y;
		// 画像１行分のデータサイズ
		const UINT rowPitch = sizeof(UINT) * (UINT)size.x;
		// 画像全体のデータサイズ
		const UINT depthPitch = rowPitch * (UINT)size.y;
		// 画像イメージ
		UINT* img = new UINT[pixelCont];
		for (UINT i = 0; i < pixelCont; i++) { img[i] = 0xff0000ff; }

		// テクスチャバッファにデータ転送
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
