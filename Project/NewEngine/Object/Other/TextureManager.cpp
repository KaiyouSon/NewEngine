#include "TextureManager.h"
#include "RenderBase.h"
#include <DirectXTex.h>
#include <d3dx12.h>
using namespace DirectX;

TextureManager::TextureManager() :
	mMutex(std::mutex{}), mSrvIncrementIndex(1), mSrvDescHeap(nullptr)
{
}

void TextureManager::Init()
{
}

// テクスチャーの取得
Texture* TextureManager::GetTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// ログ
	std::string log = "Error";

	if (GetInstance()->mTextureMap[tag].get() == nullptr)
	{
		log = "[Texture Use] Tag : " + tag + ", does not exist";
	}
	else
	{
		log = "[Texture Use] Tag : " + tag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return GetInstance()->mTextureMap[tag].get();
}

// 色を指定してテクスチャを生成しマップに格納する
Texture* TextureManager::CreateTexture(const Color color, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// テクスチャー作成
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

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
	GetInstance()->mTextureMap[tag]->size =
		Vec2((float)textureResourceDesc.Width, (float)textureResourceDesc.Height);

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&GetInstance()->mTextureMap[tag]->buffer));
	assert(SUCCEEDED(result));

	// SRV作成
	GetInstance()->CreateSRV(*GetInstance()->mTextureMap[tag], GetInstance()->mTextureMap[tag]->buffer.Get());

	// 色
	Color col = color / 255.f;

	D3D12_SUBRESOURCE_DATA subResourcesData{};
	subResourcesData.pData = (void**)&col;
	subResourcesData.RowPitch =
		(LONG_PTR)(sizeof(Color) *
			GetInstance()->mTextureMap[tag]->size.x);
	subResourcesData.SlicePitch =
		(LONG_PTR)(sizeof(Color) *
			GetInstance()->mTextureMap[tag]->size.x *
			GetInstance()->mTextureMap[tag]->size.y);

	uint64_t uploadSize = GetRequiredIntermediateSize(GetInstance()->mTextureMap[tag]->buffer.Get(), 0, 1);

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
			IID_PPV_ARGS(&GetInstance()->mTextureMap[tag]->uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		GetInstance()->mTextureMap[tag]->buffer.Get(),
		GetInstance()->mTextureMap[tag]->uploadBuffer.Get(),
		0,
		0,
		1,
		&subResourcesData);

	GetInstance()->mTextureMap[tag]->buffer->SetName(L"ResourceBuffer");
	GetInstance()->mTextureMap[tag]->uploadBuffer->SetName(L"UploadBuffer");

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = GetInstance()->mTextureMap[tag]->buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	// ログ出力
	std::string log = "[Texture Create] ColorTexture, Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());

	return GetInstance()->mTextureMap[tag].get();
}

// ファイルパスを指定してテクスチャを生成しマップの格納する
Texture* TextureManager::LoadTexture(const std::string filePath, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// テクスチャー作成
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

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
		std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", is,failed to load";
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
	GetInstance()->mTextureMap[tag]->size =
		Vec2((float)textureResourceDesc.Width, (float)textureResourceDesc.Height);

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&GetInstance()->mTextureMap[tag]->buffer));
	assert(SUCCEEDED(result));

	// SRV作成
	GetInstance()->CreateSRV(*GetInstance()->mTextureMap[tag], GetInstance()->mTextureMap[tag]->buffer.Get());

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
		GetRequiredIntermediateSize(GetInstance()->mTextureMap[tag]->buffer.Get(), 0, (UINT)metadata.mipLevels);

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
			IID_PPV_ARGS(&GetInstance()->mTextureMap[tag]->uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		GetInstance()->mTextureMap[tag]->buffer.Get(),
		GetInstance()->mTextureMap[tag]->uploadBuffer.Get(),
		0,
		0,
		(UINT)metadata.mipLevels,
		subResourcesDatas.data());

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = GetInstance()->mTextureMap[tag]->buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	// ログ出力
	std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	return GetInstance()->mTextureMap[tag].get();
}

// objファイルからロードしたテクスチャーをロードする専用関数
Texture* TextureManager::LoadMaterialTexture(const std::string filePath, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// テクスチャー作成
	GetInstance()->mMaterialTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

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
		std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", is,failed to load";
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
	GetInstance()->mMaterialTextureMap[tag]->size =
		Vec2((float)textureResourceDesc.Width, (float)textureResourceDesc.Height);

	// テクスチャバッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&GetInstance()->mMaterialTextureMap[tag]->buffer));
	assert(SUCCEEDED(result));

	// SRV作成
	GetInstance()->CreateSRV(*GetInstance()->mMaterialTextureMap[tag], GetInstance()->mMaterialTextureMap[tag]->buffer.Get());

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
		GetRequiredIntermediateSize(GetInstance()->mMaterialTextureMap[tag]->buffer.Get(), 0, (UINT)metadata.mipLevels);

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
			IID_PPV_ARGS(&GetInstance()->mMaterialTextureMap[tag]->uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		GetInstance()->mMaterialTextureMap[tag]->buffer.Get(),
		GetInstance()->mMaterialTextureMap[tag]->uploadBuffer.Get(),
		0,
		0,
		(UINT)metadata.mipLevels,
		subResourcesDatas.data());

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = GetInstance()->mMaterialTextureMap[tag]->buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	std::string log = "[MaterialTexture Load] FilePath : " + filePath + ", Tag : " + tag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	return GetInstance()->mMaterialTextureMap[tag].get();
}

// テクスチャーのアンロード関数
void TextureManager::UnLoadTexture(const std::string tag)
{
	auto it = GetInstance()->mTextureMap.find(tag);
	if (it == GetInstance()->mTextureMap.end())
	{
		return;
	}

	uint32_t index = GetInstance()->mTextureMap[tag]->GetSRVIndex();
	if (index > 0)
	{
		GetInstance()->mCheckSRVIndex[index - 1] = false;
	}

	GetInstance()->mTextureMap.erase(tag);
}

// 深度テクスチャーを生成
Texture* TextureManager::CreateDepthTexture(const Vec2 size)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	std::string tag = "DepthTexture";
	GetInstance()->mTextureMap.insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	// 深度バッファのリソース
	GetInstance()->mTextureMap[tag]->buffer = RenderBase::GetInstance()->GetDepthBuffer()->GetBufferResource()->buffer;

	// SRV作成
	GetInstance()->CreateSRV(*GetInstance()->mTextureMap[tag], GetInstance()->mTextureMap[tag]->buffer.Get());

	std::string log = "[Texture Create] DepthTexture, Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());

	return GetInstance()->mTextureMap[tag].get();
}

// レンダーテクスチャーの取得
RenderTexture* TextureManager::GetRenderTexture(const std::string tag)
{
	std::string log;
	if (GetInstance()->mRenderTextureMap[tag].get() == nullptr)
	{
		log = "[RenderTexture Use] Tag : " + tag + ", does not exist";
	}
	else
	{
		log = "[RenderTexture Use] Tag : " + tag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return GetInstance()->mRenderTextureMap[tag].get();
}

// レンダーテクスチャーを生成しマップに格納する
RenderTexture* TextureManager::CreateRenderTexture(const Vec2 size, const uint32_t num, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	std::unique_ptr<RenderTexture> renderTex = std::make_unique<RenderTexture>();
	renderTex->size = size;
	renderTex->GetBufferResources()->resize(num);

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

	for (uint32_t i = 0; i < num; i++)
	{
		result = renderBase->GetDevice()->
			CreateCommittedResource(
				&texturenResourceHeapProp,
				D3D12_HEAP_FLAG_NONE,
				&texturenResourceDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&textureResourceClearValue,
				IID_PPV_ARGS(&renderTex->GetBufferResources()->at(i).buffer));
		assert(SUCCEEDED(result));
	}

	renderTex->renderTargets.resize(num);
	renderTex->SetHandleNum(num);
	for (uint32_t i = 0; i < num; i++)
	{
		// SRV作成
		GetInstance()->CreateSRV(*renderTex, renderTex->GetBufferResources()->at(i).buffer.Get(), i);

		// RTV作成
		renderTex->renderTargets[i].GetBufferResource()->buffer = renderTex->GetBufferResources()->at(i).buffer;
		DescriptorHeapManager::GetDescriptorHeap("RTV")->CreateRTV(renderTex->renderTargets[i].GetBufferResource());
	}

	// DSV作成
	renderTex->depthBuffer.Create(size);
	renderBase->CreateDSV(renderTex->depthBuffer);

	renderTex->depthTexture = std::make_unique<Texture>();
	renderTex->depthTexture->buffer = renderTex->depthBuffer.GetBufferResource()->buffer;
	GetInstance()->CreateSRV(*renderTex->depthTexture, renderTex->depthTexture->buffer.Get());

	GetInstance()->mRenderTextureMap.insert(std::make_pair(tag, std::move(renderTex)));

	std::string log = "[RenderTexture Create] Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());

	return GetInstance()->mRenderTextureMap[tag].get();
}

// レンダーテクスチャーのアンロード関数
void TextureManager::UnLoadRenderTexture(const std::string tag)
{
	for (uint32_t i = 0; i < GetInstance()->mRenderTextureMap[tag]->GetSRVIndexes()->size(); i++)
	{
		uint32_t index = GetInstance()->mRenderTextureMap[tag]->GetSRVIndexes()->at(i);
		if (index > 0)
		{
			GetInstance()->mCheckSRVIndex[index - 1] = false;
		}
		GetInstance()->mTextureMap.erase(tag);
	}
}

// テクスチャマップの取得
std::unordered_map<std::string, std::unique_ptr<Texture>>* TextureManager::GetTextureMap()
{
	return &GetInstance()->mTextureMap;
}

// テクスチャマップの取得
std::unordered_map<std::string, std::unique_ptr<Texture>>* TextureManager::GetMaterialTextureMap()
{
	return &GetInstance()->mMaterialTextureMap;
}

// レンダーテクスチャマップの取得
std::unordered_map<std::string, std::unique_ptr<RenderTexture>>* TextureManager::GetRenderTextureMap()
{
	return &GetInstance()->mRenderTextureMap;
}

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
		CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&GetInstance()->mSrvDescHeap));
	assert(SUCCEEDED(result));
}

// SRVを作成する処理
void TextureManager::CreateSRV(Texture& texture, ID3D12Resource* buffer)
{
	// ImGUIで0番使ってあるから1から使う
	uint32_t srvIndex = 1;

	// mCheckSRVIndex使ってない番号ないかをチェック
	for (uint32_t i = 0; i < mCheckSRVIndex.size(); i++)
	{
		// i番が false だったら
		if (mCheckSRVIndex[i] == false)
		{
			srvIndex = i + 1;
			mCheckSRVIndex[i] = true;
			break;
		}
	}

	// 最後尾に新しいやつを追加
	mCheckSRVIndex.push_back(true);
	srvIndex = (uint32_t)mCheckSRVIndex.size();	// 元からindexより1多いから+1しなくていい

	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = mSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = mSrvDescHeap->GetGPUDescriptorHandleForHeapStart();

	UINT descriptorSize = RenderBase::GetInstance()->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// index分ずらす
	srvCpuHandle.ptr += (uint32_t)(descriptorSize * srvIndex);
	srvGpuHandle.ptr += (uint32_t)(descriptorSize * srvIndex);

	texture.SetCpuHandle(srvCpuHandle);
	texture.SetGpuHandle(srvGpuHandle);
	texture.SetSRVIndex(srvIndex);

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
}
void TextureManager::CreateSRV(RenderTexture& texture, ID3D12Resource* buffer, uint32_t index)
{
	// ImGUIで0番使ってあるから1から使う
	uint32_t srvIndex = 1;

	// mCheckSRVIndex使ってない番号ないかをチェック
	for (uint32_t i = 0; i < mCheckSRVIndex.size(); i++)
	{
		// i番が false だったら
		if (mCheckSRVIndex[i] == false)
		{
			srvIndex = i + 1;
			mCheckSRVIndex[i] = true;
			break;
		}
	}

	// 最後尾に新しいやつを追加
	mCheckSRVIndex.push_back(true);
	srvIndex = (uint32_t)mCheckSRVIndex.size();	// 元からindexより1多いから+1しなくていい

	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = mSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = mSrvDescHeap->GetGPUDescriptorHandleForHeapStart();

	UINT descriptorSize = RenderBase::GetInstance()->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// index分ずらす
	srvCpuHandle.ptr += (int32_t)(descriptorSize * srvIndex);
	srvGpuHandle.ptr += (int32_t)(descriptorSize * srvIndex);

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

// SRV用のディスクリプターヒープを取得
ID3D12DescriptorHeap* TextureManager::GetSrvDescHeap()
{
	return mSrvDescHeap.Get();
}
