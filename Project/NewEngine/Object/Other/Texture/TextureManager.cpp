#include "TextureManager.h"
#include "RenderBase.h"
#include <DirectXTex.h>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

using namespace DirectX;

TextureManager::TextureManager() : mMutex(std::mutex{})
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- 生成関連 ------------------------------------------------------------------------------------------------------ ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// テクスチャーをロード
void TextureManager::LoadTexture(const std::string filePath, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// マップに格納
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	Texture* texture = dynamic_cast<Texture*>(GetInstance()->mTextureMap[tag].get());

	std::string path = "Application/Resources/Texture/" + filePath;

	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(path.begin(), path.end());

	// WICを使用してテクスチャデータを読み込む
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", is,failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "テクスチャ読み込みに失敗しました");
	}

	// ミップマップを生成する
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

	// テクスチャのフォーマットをSRGBに設定
	metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			static_cast<uint64_t>(metadata.width),
			static_cast<uint32_t>(metadata.height),
			static_cast<uint16_t>(metadata.arraySize),
			static_cast<uint16_t>(metadata.mipLevels),
			1);

	// テクスチャーのバッファ生成
	texture->Create(resourceDesc, static_cast<uint32_t>(metadata.mipLevels));

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(texture->GetBufferResource());

	// サブリソースデータを初期化
	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas(metadata.mipLevels);
	for (size_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// イメージデータを取得してサブリソースデータを設定
		const Image* img = scratchImg.GetImage(i, 0, 0);
		subResourcesDatas[i].pData = img->pixels;
		subResourcesDatas[i].RowPitch = img->rowPitch;
		subResourcesDatas[i].SlicePitch = img->slicePitch;

	}
	// テクスチャーをアップロード
	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		texture->GetBufferResource()->buffer.Get(),
		texture->GetUploadBuffer()->GetBufferResource()->buffer.Get(),
		0,
		0,
		static_cast<uint32_t>(metadata.mipLevels),
		subResourcesDatas.data());

	// リソースの状態変更
	RenderBase::GetInstance()->TransitionBufferState(
		texture->GetBufferResource(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	// 使い終わったならテクスチャにmove
	texture->SetScratchImage(&scratchImg);

	// Log出力
	std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", was loaded successfully";
	OutputDebugLog(log.c_str());
}
void TextureManager::LoadTextureFromDDS(const std::string filePath, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// マップに格納
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	Texture* texture = dynamic_cast<Texture*>(GetInstance()->mTextureMap[tag].get());

	std::string path = "Application/Resources/DDSTexture/" + filePath;

	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(path.begin(), path.end());

	// WICを使用してテクスチャデータを読み込む
	result = LoadFromDDSFile(
		wfilePath.c_str(),
		DDS_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", is,failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "テクスチャ読み込みに失敗しました");
	}

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			static_cast<uint64_t>(metadata.width),
			static_cast<uint32_t>(metadata.height),
			static_cast<uint16_t>(metadata.arraySize),
			static_cast<uint16_t>(metadata.mipLevels),
			1);

	// テクスチャーのバッファ生成
	texture->Create(resourceDesc, static_cast<uint32_t>(metadata.mipLevels));

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(texture->GetBufferResource());

	// サブリソースデータを初期化
	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas(metadata.mipLevels);
	for (size_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// イメージデータを取得してサブリソースデータを設定
		const Image* img = scratchImg.GetImage(i, 0, 0);
		subResourcesDatas[i].pData = img->pixels;
		subResourcesDatas[i].RowPitch = img->rowPitch;
		subResourcesDatas[i].SlicePitch = img->slicePitch;

	}
	// テクスチャーをアップロード
	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		texture->GetBufferResource()->buffer.Get(),
		texture->GetUploadBuffer()->GetBufferResource()->buffer.Get(),
		0,
		0,
		static_cast<uint32_t>(metadata.mipLevels),
		subResourcesDatas.data());

	// リソースの状態変更
	RenderBase::GetInstance()->TransitionBufferState(
		texture->GetBufferResource(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	// 使い終わったならテクスチャにmove
	texture->SetScratchImage(&scratchImg);

	// Log出力
	std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", was loaded successfully";
	OutputDebugLog(log.c_str());
}

// mtlファイルのテクスチャーをロード
Texture* TextureManager::LoadMaterialTexture(const std::string filePath, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// 繝・け繧ｹ繝√Ε繝ｼ菴懈・
	GetInstance()->mMaterialTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	Texture* texture = dynamic_cast<Texture*>(GetInstance()->mMaterialTextureMap[tag].get());

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(filePath.begin(), filePath.end());

	HRESULT result;

	// WICを使用してテクスチャデータを読み込む
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		std::string log = "[MaterialTexture Load] FilePath : " + filePath + ", Tag : " + tag + ", is,failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "mtlテクスチャ読み込みに失敗しました");
	}

	// ミップマップを生成する
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

	// テクスチャのフォーマットをSRGBに設定
	metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			static_cast<uint64_t>(metadata.width),
			static_cast<uint32_t>(metadata.height),
			static_cast<uint16_t>(metadata.arraySize),
			static_cast<uint16_t>(metadata.mipLevels),
			1);

	// テクスチャーのバッファ生成
	texture->Create(resourceDesc, static_cast<uint32_t>(metadata.mipLevels));

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(texture->GetBufferResource());

	// サブリソースデータを初期化
	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas(metadata.mipLevels);
	for (size_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// イメージデータを取得してサブリソースデータを設定
		const Image* img = scratchImg.GetImage(i, 0, 0);
		subResourcesDatas[i].pData = img->pixels;
		subResourcesDatas[i].RowPitch = img->rowPitch;
		subResourcesDatas[i].SlicePitch = img->slicePitch;
	}

	// テクスチャーをアップロード
	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		texture->GetBufferResource()->buffer.Get(),
		texture->GetUploadBuffer()->GetBufferResource()->buffer.Get(),
		0,
		0,
		static_cast<uint32_t>(metadata.mipLevels),
		subResourcesDatas.data());

	// リソースの状態変更
	RenderBase::GetInstance()->TransitionBufferState(
		texture->GetBufferResource(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	std::string log = "[MaterialTexture Load] FilePath : " + filePath + ", Tag : " + tag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	return texture;
}

// 1x1の色テクスチャーを作成
void TextureManager::CreateColorTexture(const Color color, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// マップに格納
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, 1, 1, 1);

	Texture* texture = dynamic_cast<Texture*>(GetInstance()->mTextureMap[tag].get());

	// テクスチャーのバッファ生成
	texture->Create(resourceDesc);

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(texture->GetBufferResource());

	// サブリソースデータを初期化
	Color col = color / 255.f;
	Vec2 size = Vec2(texture->GetInitalSize().x, texture->GetInitalSize().y);
	D3D12_SUBRESOURCE_DATA subResourcesData;
	subResourcesData.pData = (void**)&col;
	subResourcesData.RowPitch = static_cast<int64_t>(sizeof(Color) * size.x);
	subResourcesData.SlicePitch = static_cast<int64_t>(sizeof(Color) * size.x * size.y);

	// テクスチャーをアップロード
	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		texture->GetBufferResource()->buffer.Get(),
		texture->GetUploadBuffer()->GetBufferResource()->buffer.Get(),
		0,
		0,
		1,
		&subResourcesData);

	// リソースの状態変更
	RenderBase::GetInstance()->TransitionBufferState(
		texture->GetBufferResource(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	// Log出力
	std::string log = "[Texture Create] ColorTexture, Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());
}

// 深度テクスチャの作成
void TextureManager::CreateDepthTexture(DepthBuffer* depthBuffer, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// マップに格納
	GetInstance()->mDepthTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<DepthTexture>())));

	DepthTexture* texture = dynamic_cast<DepthTexture*>(
		GetInstance()->mDepthTextureMap[tag].get());

	// 深度テクスチャのバッファ生成
	texture->Create(depthBuffer);

	std::string log = "[DepthTexture Create] DepthTexture, Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());
}

// ボリュームテクスチャの作成
void TextureManager::CreateVolumeTexture(const std::vector<Texture*>& texs, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// マップに格納
	GetInstance()->mVolumeTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<VolumeTexture>())));

	VolumeTexture* texture = dynamic_cast<VolumeTexture*>(
		GetInstance()->mVolumeTextureMap[tag].get());

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex3D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			static_cast<uint64_t>(texs[0]->GetInitalSize().x),
			static_cast<uint32_t>(texs[0]->GetInitalSize().y),
			static_cast<uint16_t>(texs.size()),
			1);
	// バッファ生成
	texture->Create(resourceDesc);

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(texture->GetBufferResource());

	// イメージデータを取得してサブリソースデータを設定
	D3D12_SUBRESOURCE_DATA subResourcesData;
	subResourcesData.pData = nullptr;
	for (uint32_t i = 0; i < texs.size(); i++)
	{
		const Image* img = texs[i]->GetScratchImage()->GetImage(0, 0, 0);
		subResourcesData.RowPitch = img->rowPitch;
		subResourcesData.SlicePitch = img->slicePitch;

		// pData の初期化を行う
		if (subResourcesData.pData == nullptr)
		{
			subResourcesData.pData = new BYTE[subResourcesData.SlicePitch * texs.size()];
		}

		// テクスチャデータを subResourcesData.pData にコピー
		BYTE* targetData = reinterpret_cast<BYTE*>(const_cast<void*>(subResourcesData.pData)) + i * img->slicePitch;
		memcpy(targetData, img->pixels, img->slicePitch);
	}
	// テクスチャーをアップロード
	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		texture->GetBufferResource()->buffer.Get(),
		texture->GetUploadBuffer()->GetBufferResource()->buffer.Get(),
		0,
		0,
		1,
		&subResourcesData);

	// アップロード後にdeleteする
	delete[] subResourcesData.pData;

	// リソースの状態変更
	RenderBase::GetInstance()->TransitionBufferState(
		texture->GetBufferResource(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	std::string log = "[VolumeTexture Create] Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());
}

// レンダーテクスチャの作成
void TextureManager::CreateRenderTexture(const Vec2 size, const uint32_t rtvNum, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// マップに格納
	GetInstance()->mRenderTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<RenderTexture>())));

	RenderTexture* texture = dynamic_cast<RenderTexture*>(
		GetInstance()->mRenderTextureMap[tag].get());

	// 生成
	texture->Create(size, rtvNum);

	std::string log = "[RenderTexture Create] Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- 破棄関連 ------------------------------------------------------------------------------------------------------ ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// テクスチャの破棄
void TextureManager::DestroyTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mTextureMap.find(tag);
	if (it == GetInstance()->mTextureMap.end())
	{
		std::string log;
		log = "[Texture Error] Tag : " + tag + ", is nullptr";
		return;
	}

	// SRV解放
	DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(
		GetInstance()->mTextureMap[tag]->GetBufferResource());

	// Mapから削除
	GetInstance()->mTextureMap.erase(tag);
}

// マテリアルのテクスチャの破棄
void TextureManager::DestroyMaterialTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mMaterialTextureMap.find(tag);
	if (it == GetInstance()->mMaterialTextureMap.end())
	{
		std::string log;
		log = "[Material Texture Error] Tag : " + tag + ", is nullptr";
		return;
	}

	// SRV解放
	DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(
		GetInstance()->mMaterialTextureMap[tag]->GetBufferResource());

	// Mapから削除
	GetInstance()->mMaterialTextureMap.erase(tag);
}

// 深度テクスチャの破棄
void TextureManager::DestroyDepthTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mDepthTextureMap.find(tag);
	if (it == GetInstance()->mDepthTextureMap.end())
	{
		std::string log;
		log = "[Texture Error] Tag : " + tag + ", is nullptr";
		return;
	}

	// SRV解放
	DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(
		GetInstance()->mDepthTextureMap[tag]->GetBufferResource());

	// Mapから削除
	GetInstance()->mDepthTextureMap.erase(tag);
}

// ボリュームテクスチャの破棄
void TextureManager::DestroyVolumeTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mVolumeTextureMap.find(tag);
	if (it == GetInstance()->mVolumeTextureMap.end())
	{
		std::string log;
		log = "[Texture Error] Tag : " + tag + ", is nullptr";
		return;
	}

	// SRV解放
	DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(
		GetInstance()->mVolumeTextureMap[tag]->GetBufferResource());

	// Mapから削除
	GetInstance()->mVolumeTextureMap.erase(tag);
}

// レンダーテクスチャの破棄
void TextureManager::DestroyRenderTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mRenderTextureMap.find(tag);
	if (it == GetInstance()->mRenderTextureMap.end())
	{
		std::string log;
		log = "[RenderTexture Error] Tag : " + tag + ", is nullptr";
		return;
	}

	RenderTexture* texture = GetInstance()->mRenderTextureMap[tag].get();

	for (uint32_t i = 0; i < texture->GetBufferResources()->size(); i++)
	{
		// SRV解放
		DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(&texture->GetBufferResources()->at(i));
		// RTV解放
		DescriptorHeapManager::GetDescriptorHeap("RTV")->DestroyView(&texture->GetBufferResources()->at(i));
	}

	// 深度テクスチャを使った場合
	if (texture->useDepth == true)
	{
		// SRV解放
		DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(texture->GetDepthTexture()->GetBufferResource());

		// DSV解放
		DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(texture->GetDepthBuffer()->GetBufferResource());
	}

	// Mapから削除
	GetInstance()->mRenderTextureMap.erase(tag);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- テクスチャの取得関連 ------------------------------------------------------------------------------------------ ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// テクスチャーを取得
Texture* TextureManager::GetTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mTextureMap.find(tag);
	if (it == GetInstance()->mTextureMap.end())
	{
		std::string log;
		log = "[Texture Error] Tag : " + tag + ", is nullptr";
		return nullptr;
	}
	else
	{
		std::string log;
		log = "[Texture Use] Tag : " + tag + ", was used";
		OutputDebugLog(log.c_str());

		Texture* texture = dynamic_cast<Texture*>(
			GetInstance()->mTextureMap[tag].get());
		return texture;
	}
}

// 深度テクスチャーを取得
DepthTexture* TextureManager::GetDepthTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mDepthTextureMap.find(tag);
	if (it == GetInstance()->mDepthTextureMap.end())
	{
		std::string log;
		log = "[DepthTexture Error] Tag : " + tag + ", is nullptr";
		return nullptr;
	}
	else
	{
		std::string log;
		log = "[DepthTexture Use] Tag : " + tag + ", was used";
		OutputDebugLog(log.c_str());

		DepthTexture* texture = dynamic_cast<DepthTexture*>(
			GetInstance()->mDepthTextureMap[tag].get());
		return texture;
	}
}

// ボリュームテクスチャを取得
VolumeTexture* TextureManager::GetVolumeTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mVolumeTextureMap.find(tag);
	if (it == GetInstance()->mVolumeTextureMap.end())
	{
		std::string log;
		log = "[VolumeTexture Error] Tag : " + tag + ", is nullptr";
		return nullptr;
	}
	else
	{
		std::string log;
		log = "[VolumeTexture Use] Tag : " + tag + ", was used";
		OutputDebugLog(log.c_str());

		VolumeTexture* texture = dynamic_cast<VolumeTexture*>(
			GetInstance()->mVolumeTextureMap[tag].get());
		return texture;
	}
}

// レンダーテクスチャを取得
RenderTexture* TextureManager::GetRenderTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mRenderTextureMap.find(tag);
	if (it == GetInstance()->mRenderTextureMap.end())
	{
		std::string log;
		log = "[RenderTexture Error] Tag : " + tag + ", is nullptr";
		return nullptr;
	}
	else
	{
		std::string log;
		log = "[RenderTexture Use] Tag : " + tag + ", was used";
		OutputDebugLog(log.c_str());

		RenderTexture* texture = dynamic_cast<RenderTexture*>(
			GetInstance()->mRenderTextureMap[tag].get());
		return texture;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- マップの取得関連 ---------------------------------------------------------------------------------------------- ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// テクスチャーマップを取得
std::unordered_map<std::string, std::unique_ptr<Texture>>* TextureManager::GetTextureMap()
{
	return &GetInstance()->mTextureMap;
}

// マテリアルテクスチャーマップを取得
std::unordered_map<std::string, std::unique_ptr<Texture>>* TextureManager::GetMaterialTextureMap()
{
	return &GetInstance()->mMaterialTextureMap;
}

// 深度テクスチャーマップを取得
std::unordered_map<std::string, std::unique_ptr<DepthTexture>>* TextureManager::GetDepthTextureMap()
{
	return &GetInstance()->mDepthTextureMap;
}

// ボリュームテクスチャーマップを取得
std::unordered_map<std::string, std::unique_ptr<VolumeTexture>>* TextureManager::GetVolumeTextureMap()
{
	return &GetInstance()->mVolumeTextureMap;
}

// レンダーテクスチャーマップを取得
std::unordered_map<std::string, std::unique_ptr<RenderTexture>>* TextureManager::GetRenderTextureMap()
{
	return &GetInstance()->mRenderTextureMap;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- その他 -------------------------------------------------------------------------------------------------------- ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// コマンド実行
void TextureManager::ExcuteComandList()
{
	ID3D12GraphicsCommandList* iCommandList = RenderBase::GetInstance()->GetCommandList();

	iCommandList->Close();

	ID3D12CommandQueue* iCommandQueue = RenderBase::GetInstance()->GetCommandQueue();

	ID3D12CommandList* list[] = { iCommandList };
	iCommandQueue->ExecuteCommandLists(1, list);

	RenderBase::GetInstance()->PreIncrimentFenceValue();

	// コマンドキューにシグナルを送信
	iCommandQueue->Signal(RenderBase::GetInstance()->GetFence(), RenderBase::GetInstance()->GetFenceValue());

	if (RenderBase::GetInstance()->GetFence()->GetCompletedValue() != RenderBase::GetInstance()->GetFenceValue())
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		RenderBase::GetInstance()->GetFence()->SetEventOnCompletion(RenderBase::GetInstance()->GetFenceValue(), event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	HRESULT result;

	// コマンドアロケータをリセット
	result = RenderBase::GetInstance()->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
	// コマンドリストをリセットし、コマンドアロケータを関連付けます
	result = iCommandList->Reset(RenderBase::GetInstance()->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}

