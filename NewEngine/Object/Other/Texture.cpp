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
	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

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
			//D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&buffer));
	assert(SUCCEEDED(result));

	buffer->SetName(L"TextureBuffer");

	RenderBase::GetInstance()->GetDevice()->
		GetCopyableFootprints(&textureResourceDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(total_bytes);

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&uploadBuffer));
	assert(SUCCEEDED(result));

	Color* ptr = nullptr;
	uploadBuffer->Map(0, nullptr, (void**)&ptr);
	assert(SUCCEEDED(result));
	Color col = { 1,1,1,1 };
	memcpy(ptr, &col, sizeof(Color));

	UploadHeap(16);

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
	CD3DX12_HEAP_PROPERTIES textureHeapProp =
		CD3DX12_HEAP_PROPERTIES(
			D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0);

	// リソース設定
	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			(UINT64)metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels,
			1);

	// テクスチャのサイズをセット
	size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			//D3D12_RESOURCE_STATE_COPY_DEST,
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


	//RenderBase::GetInstance()->GetDevice()->
//	GetCopyableFootprints(&textureResourceDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	//// ヒープの設定
	//D3D12_HEAP_PROPERTIES textureHeapProp1{};
	//textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	//CD3DX12_RESOURCE_DESC textureResourceDesc1 =
	//	CD3DX12_RESOURCE_DESC::Buffer(total_bytes);

	//// テクスチャバッファの生成
	//result = RenderBase::GetInstance()->GetDevice()->
	//	CreateCommittedResource(
	//		&textureHeapProp1,
	//		D3D12_HEAP_FLAG_NONE,
	//		&textureResourceDesc1,
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&uploadBuffer));
	//assert(SUCCEEDED(result));

	//// = scratchImg.GetImage(0, 0, 0);
	//int test = sizeof(Image) * scratchImg.GetImageCount();

	//const Image* img = nullptr;
	//uploadBuffer->Map(0, nullptr, (void**)&img);
	//img = scratchImg.GetImages();

	//uint8_t* pixel = nullptr;
	//uploadBuffer->Map(0, nullptr, (void**)&pixel);
	//pixel = scratchImg.GetPixels();

	// 全ミップマップについて
	//for (size_t i = 0; i < metadata.mipLevels; i++)
	//{
	//	// 全ミップマップレベルを指定してイメージを取得
	//	//img = scratchImg.GetImage(i, 0, 0);

	//	const Image* scratchImage = scratchImg.GetImage(i, 0, 0);
	//	memcpy(img->pixels, &scratchImage, sizeof(Image));


	//	//// テクスチャバッファにデータ転送
	//	//result = buffer->WriteToSubresource(
	//	//	(UINT)i,
	//	//	nullptr,				// 全領域へコピー
	//	//	img->pixels,			// 元データアドレス
	//	//	(UINT)img->rowPitch,	// １ラインサイズ
	//	//	(UINT)img->slicePitch	// １枚サイズ
	//	//);
	//	//assert(SUCCEEDED(result));
	//}

	//UploadHeap(16);

	RenderBase::GetInstance()->CreateSRV(*this, textureResourceDesc);
}

void Texture::UploadHeap(const Vec2& size)
{
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

	RenderBase::GetInstance()->PreIncrimentFenceValue();

	// コマンドの実行完了を待つ
	iCommandQueue->Signal(RenderBase::GetInstance()->GetFence(), RenderBase::GetInstance()->GetFenceValue());

	auto test = RenderBase::GetInstance()->GetFence()->GetCompletedValue();
	if (RenderBase::GetInstance()->GetFence()->GetCompletedValue() != RenderBase::GetInstance()->GetFenceValue())
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		RenderBase::GetInstance()->GetFence()->SetEventOnCompletion(RenderBase::GetInstance()->GetFenceValue(), event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// キューをクリア
	result = RenderBase::GetInstance()->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
	result = iCommandList->Reset(RenderBase::GetInstance()->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));

	//memset(&barrier1, 0, sizeof(barrie1));

	//iCommandList->ResourceBarrier(1, &barrier1);
}
