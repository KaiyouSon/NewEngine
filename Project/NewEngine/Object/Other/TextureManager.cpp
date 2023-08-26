#include "TextureManager.h"
#include "RenderBase.h"
#include <DirectXTex.h>
#include <d3dx12.h>
using namespace DirectX;

#pragma region 静的メンバー変数

// srv作成時にインクリメント用
UINT TextureManager::sSrvIncrementIndex = 1;

// srv用ディスクリプタヒープ
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> TextureManager::sSrvDescHeap;

// テクスチャーのマップ
std::unordered_map<std::string, std::unique_ptr<Texture>> TextureManager::sTextureMap;

// レンダーテクスチャーのマップ
std::unordered_map<std::string, std::unique_ptr<RenderTexture>> TextureManager::sRenderTextureMap;

// 排他制御
std::mutex TextureManager::sMtx = std::mutex{};

#pragma endregion

#pragma region テクスチャー関連

// テクスチャーの取得
Texture* TextureManager::GetTexture(std::string textureTag)
{
	std::string log;
	if (sTextureMap[textureTag].get() == nullptr)
	{
		log = "[Texture Use] Tag : " + textureTag + ", does not exist";
	}
	else
	{
		log = "[Texture Use] Tag : " + textureTag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return sTextureMap[textureTag].get();
}

// 色を指定してテクスチャを生成する
Texture TextureManager::CreateTexture(Color color)
{
	Texture tex;

	HRESULT result;

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
	tex.size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&tex.buffer));
	assert(SUCCEEDED(result));

	CreateSRV(tex, tex.buffer.Get());

	Color col = Color(color.r / 255, color.g / 255, color.b / 255, color.a / 255);

	D3D12_SUBRESOURCE_DATA subResourcesData{};
	subResourcesData.pData = (void**)&col;
	subResourcesData.RowPitch = (LONG_PTR)(sizeof(Color) * tex.size.x);
	subResourcesData.SlicePitch = (LONG_PTR)(sizeof(Color) * tex.size.x * tex.size.y);

	uint64_t uploadSize = GetRequiredIntermediateSize(tex.buffer.Get(), 0, 1);

	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&tex.uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		tex.buffer.Get(),
		tex.uploadBuffer.Get(),
		0,
		0,
		1,
		&subResourcesData);

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = tex.buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	std::string log = "[Texture Create] ColorTexture, created";
	OutputDebugLog(log.c_str());

	ExcuteComandList();

	return tex;
}

// 色を指定してテクスチャを生成しマップに格納する
Texture* TextureManager::CreateTexture(Color color, std::string textureTag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx);

	sTextureMap.insert(std::make_pair(textureTag, std::move(std::make_unique<Texture>())));

	HRESULT result;

	// ヒープの設定
	CD3DX12_HEAP_PROPERTIES textureHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// リソース設定
	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1, 1, 1, 1, 1);

	// テクスチャのサイズをセット
	sTextureMap[textureTag]->size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&sTextureMap[textureTag]->buffer));
	assert(SUCCEEDED(result));

	CreateSRV(*sTextureMap[textureTag], sTextureMap[textureTag]->buffer.Get());

	Color col = Color(color.r / 255, color.g / 255, color.b / 255, color.a / 255);

	D3D12_SUBRESOURCE_DATA subResourcesData{};
	subResourcesData.pData = (void**)&col;
	subResourcesData.RowPitch = (LONG_PTR)(sizeof(Color) * sTextureMap[textureTag]->size.x);
	subResourcesData.SlicePitch = (LONG_PTR)(sizeof(Color) * sTextureMap[textureTag]->size.x * sTextureMap[textureTag]->size.y);

	uint64_t uploadSize = GetRequiredIntermediateSize(sTextureMap[textureTag]->buffer.Get(), 0, 1);

	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&sTextureMap[textureTag]->uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		sTextureMap[textureTag]->buffer.Get(),
		sTextureMap[textureTag]->uploadBuffer.Get(),
		0,
		0,
		1,
		&subResourcesData);

	sTextureMap[textureTag]->buffer->SetName(L"ResourceBuffer");
	sTextureMap[textureTag]->uploadBuffer->SetName(L"UploadBuffer");

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = sTextureMap[textureTag]->buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	std::string log = "[Texture Create] ColorTexture, Tag : " + textureTag + ", created";
	OutputDebugLog(log.c_str());

	return sTextureMap[textureTag].get();
}

// ファイルパスを指定してテクスチャを生成する
Texture TextureManager::LoadTexture(std::string filePath)
{
	Texture tex;

	std::string path = "Application/Resources/Texture/" + filePath;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(path.begin(), path.end());

	HRESULT result;

	// WICテクスチャのロード
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		std::string log = "[Texture Load] FilePath : " + filePath + ", is,failed to load";
		OutputDebugLog(log.c_str());

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
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

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
	tex.size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&tex.buffer));
	assert(SUCCEEDED(result));

	CreateSRV(tex, tex.buffer.Get());

	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas{};
	subResourcesDatas.resize(metadata.mipLevels);

	for (size_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// 全ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		subResourcesDatas[i].pData = img->pixels;
		subResourcesDatas[i].RowPitch = img->rowPitch;
		subResourcesDatas[i].SlicePitch = img->slicePitch;
	}

	uint64_t uploadSize = GetRequiredIntermediateSize(tex.buffer.Get(), 0, (UINT)metadata.mipLevels);

	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;

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

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		tex.buffer.Get(),
		uploadBuffer.Get(),
		0,
		0,
		(UINT)metadata.mipLevels,
		subResourcesDatas.data());

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = tex.buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	std::string log = "[Texture Load] FilePath : " + filePath + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	ExcuteComandList();

	return tex;
}

// ファイルパスを指定してテクスチャを生成しマップの格納する
Texture* TextureManager::LoadTexture(std::string filePath, std::string textureTag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx);

	sTextureMap.insert(std::make_pair(textureTag, std::move(std::make_unique<Texture>())));

	std::string path = "Application/Resources/Texture/" + filePath;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(path.begin(), path.end());

	HRESULT result;

	// WICテクスチャのロード
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + textureTag + ", is,failed to load";
		OutputDebugLog(log.c_str());

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
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

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
	sTextureMap[textureTag]->size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&sTextureMap[textureTag]->buffer));
	assert(SUCCEEDED(result));

	sTextureMap[textureTag]->buffer->SetName(L"TextureBuffer");

	CreateSRV(*sTextureMap[textureTag], sTextureMap[textureTag]->buffer.Get());

	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas{};
	subResourcesDatas.resize(metadata.mipLevels);

	for (size_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// 全ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		subResourcesDatas[i].pData = img->pixels;
		subResourcesDatas[i].RowPitch = img->rowPitch;
		subResourcesDatas[i].SlicePitch = img->slicePitch;
	}

	uint64_t uploadSize = GetRequiredIntermediateSize(sTextureMap[textureTag]->buffer.Get(), 0, (UINT)metadata.mipLevels);

	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&sTextureMap[textureTag]->uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		sTextureMap[textureTag]->buffer.Get(),
		sTextureMap[textureTag]->uploadBuffer.Get(),
		0,
		0,
		(UINT)metadata.mipLevels,
		subResourcesDatas.data());

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = sTextureMap[textureTag]->buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + textureTag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	return sTextureMap[textureTag].get();
}

// objファイルからロードしたテクスチャーをロードする専用関数
Texture* TextureManager::LoadMaterialTexture(std::string filePath, std::string textureTag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx);

	sTextureMap.insert(std::make_pair(textureTag, std::move(std::make_unique<Texture>())));

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(filePath.begin(), filePath.end());

	HRESULT result;

	// WICテクスチャのロード
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + textureTag + ", is,failed to load";
		OutputDebugLog(log.c_str());

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
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

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
	sTextureMap[textureTag]->size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&sTextureMap[textureTag]->buffer));
	assert(SUCCEEDED(result));

	CreateSRV(*sTextureMap[textureTag], sTextureMap[textureTag]->buffer.Get());

	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas{};
	subResourcesDatas.resize(metadata.mipLevels);

	for (size_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// 全ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		subResourcesDatas[i].pData = img->pixels;
		subResourcesDatas[i].RowPitch = img->rowPitch;
		subResourcesDatas[i].SlicePitch = img->slicePitch;
	}

	uint64_t uploadSize =
		GetRequiredIntermediateSize(sTextureMap[textureTag]->buffer.Get(), 0, (UINT)metadata.mipLevels);

	// ヒープの設定
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&sTextureMap[textureTag]->uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		sTextureMap[textureTag]->buffer.Get(),
		sTextureMap[textureTag]->uploadBuffer.Get(),
		0,
		0,
		(UINT)metadata.mipLevels,
		subResourcesDatas.data());

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = sTextureMap[textureTag]->buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + textureTag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	return sTextureMap[textureTag].get();
}

// 深度テクスチャーを生成
Texture* TextureManager::CreateDepthTexture(Vec2 size)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx);

	std::string tag = "DepthTexture";
	sTextureMap.insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	// 深度バッファのリソース
	sTextureMap[tag]->buffer = RenderBase::GetInstance()->GetDepthBuffer()->GetBuffer();
	//sTextureMap[tag]->buffer->SetName(L"DepthBuffer");

	// SRV作成
	CreateSRV(*sTextureMap[tag], sTextureMap[tag]->buffer.Get());

	std::string log = "[Texture Create] DepthTexture, Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());

	return sTextureMap[tag].get();
}

#pragma endregion

#pragma region レンダーテクスチャー関連

// レンダーテクスチャーの取得
RenderTexture* TextureManager::GetRenderTexture(std::string textureTag)
{
	std::string log;
	if (sRenderTextureMap[textureTag].get() == nullptr)
	{
		log = "[RenderTexture Use] Tag : " + textureTag + ", does not exist";
	}
	else
	{
		log = "[RenderTexture Use] Tag : " + textureTag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return sRenderTextureMap[textureTag].get();
}

// レンダーテクスチャーを生成しマップに格納する
RenderTexture* TextureManager::CreateRenderTexture(Vec2 size, uint32_t num, std::string textureTag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx);

	std::unique_ptr<RenderTexture> renderTex = std::make_unique<RenderTexture>();
	renderTex->buffers.resize(num);


	HRESULT result;
	RenderBase* renderBase = RenderBase::GetInstance();

	// ヒープ設定
	CD3DX12_HEAP_PROPERTIES texturenResourceHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// リソース設定
	CD3DX12_RESOURCE_DESC texturenResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			(UINT64)size.x, (UINT)size.y,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_CLEAR_VALUE textureResourceClearValue =
		CD3DX12_CLEAR_VALUE(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			RenderTexture::sClearColor);

	renderTex->size = size;

	for (uint32_t i = 0; i < num; i++)
	{
		result = renderBase->GetDevice()->
			CreateCommittedResource(
				&texturenResourceHeapProp,
				D3D12_HEAP_FLAG_NONE,
				&texturenResourceDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&textureResourceClearValue,
				IID_PPV_ARGS(&renderTex->buffers[i]));
		assert(SUCCEEDED(result));
	}

	//renderTex->buffers[0]->SetName(L"RenderTex1");

	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	renderTex->renderTargets.resize(num);
	renderTex->SetHandleNum(num);
	for (uint32_t i = 0; i < num; i++)
	{
		// SRV作成
		CreateSRV(*renderTex, renderTex->buffers[i].Get(), i);

		// RTV作成
		renderTex->renderTargets[i].buffer_ = renderTex->buffers[i];
		renderBase->CreateRTV(renderTex->renderTargets[i], &rtvDesc);
	}

	// DSV作成
	renderTex->depthBuffer.Create(size);
	renderBase->CreateDSV(renderTex->depthBuffer);
	renderTex->depthBuffer.mBuffer->SetName(L"DepthBuffer");

	renderTex->depthTexture = std::make_unique<Texture>();
	renderTex->depthTexture->buffer = renderTex->depthBuffer.GetBuffer();
	CreateSRV(*renderTex->depthTexture, renderTex->depthTexture->buffer.Get());

	sRenderTextureMap.insert(std::make_pair(textureTag, std::move(renderTex)));

	std::string log = "[RenderTexture Create] Tag : " + textureTag + ", created";
	OutputDebugLog(log.c_str());

	return sRenderTextureMap[textureTag].get();
}

#pragma endregion

std::unordered_map<std::string, std::unique_ptr<Texture>>* TextureManager::GetTextureMap()
{
	return &sTextureMap;
}

std::unordered_map<std::string, std::unique_ptr<RenderTexture>>* TextureManager::GetRenderTextureMap()
{
	return &sRenderTextureMap;
}

#pragma region その他の処理

// ディスクリプターヒープを作成する処理
void TextureManager::CreateDescriptorHeap()
{
	HRESULT result;

	// --- SRV ------------------------------------------------------ //
	size_t maxSRVCount = 2056;	// SRVの最大個数

	// SRV用デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
	srvHeapDesc.NumDescriptors = (UINT)maxSRVCount;

	// SRV用デスクリプタヒープを生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&sSrvDescHeap));
	assert(SUCCEEDED(result));

}

// SRVを作成する処理
void TextureManager::CreateSRV(Texture& texture, ID3D12Resource* buffer)
{
	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = sSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = sSrvDescHeap->GetGPUDescriptorHandleForHeapStart();

	UINT descriptorSize = RenderBase::GetInstance()->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	srvCpuHandle.ptr += (int32_t)(descriptorSize * sSrvIncrementIndex);
	srvGpuHandle.ptr += (int32_t)(descriptorSize * sSrvIncrementIndex);

	texture.SetCpuHandle(srvCpuHandle);
	texture.SetGpuHandle(srvGpuHandle);

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// srv設定構造体
	if (buffer->GetDesc().Format == DXGI_FORMAT_D32_FLOAT)
	{
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	}
	else
	{
		srvDesc.Format = buffer->GetDesc().Format;
	}
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2Dテクスチャ
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;	// 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = buffer->GetDesc().MipLevels;

	// ハンドルの指す位置にシェーダーリソースビュー作成
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(buffer, &srvDesc, srvCpuHandle);

	sSrvIncrementIndex++;
}
void TextureManager::CreateSRV(RenderTexture& texture, ID3D12Resource* buffer, uint32_t index)
{
	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = sSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = sSrvDescHeap->GetGPUDescriptorHandleForHeapStart();

	UINT descriptorSize = RenderBase::GetInstance()->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	srvCpuHandle.ptr += (int32_t)(descriptorSize * sSrvIncrementIndex);
	srvGpuHandle.ptr += (int32_t)(descriptorSize * sSrvIncrementIndex);

	texture.SetCpuHandle(index, srvCpuHandle);
	texture.SetGpuHandle(index, srvGpuHandle);

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// srv設定構造体
	srvDesc.Format = buffer->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = buffer->GetDesc().MipLevels;

	// ハンドルの指す位置にシェーダーリソースビュー作成
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(buffer, &srvDesc, srvCpuHandle);

	sSrvIncrementIndex++;
}

// テクスチャーロード後のコマンドリストの実行
void TextureManager::ExcuteComandList()
{
	ID3D12GraphicsCommandList* iCommandList = RenderBase::GetInstance()->GetCommandList();

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

	HRESULT result;

	// キューをクリア
	result = RenderBase::GetInstance()->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
	result = iCommandList->Reset(RenderBase::GetInstance()->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}

#pragma endregion
