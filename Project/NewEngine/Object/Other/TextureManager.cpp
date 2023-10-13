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

void TextureManager::Init()
{
}

// 1x1の色テクスチャーを生成する関数
void TextureManager::CreateTexture(const Color color, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// マップに格納
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	Texture* texture = dynamic_cast<Texture*>(GetInstance()->mTextureMap[tag].get());

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, 1, 1, 1);

	// テクスチャーのバッファ生成
	texture->Create(resourceDesc);

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(texture->GetBufferResource());

	// サブリソースデータを初期化
	Color col = color / 255.f;
	Vec2 size = texture->GetInitalSize();
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

// テクスチャーをロードする関数
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

// mtlファイルのテクスチャーをロードする関数
Texture* TextureManager::LoadMaterialTexture(const std::string filePath, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// 繝・け繧ｹ繝√Ε繝ｼ菴懈・
	GetInstance()->mMaterialTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	Texture* texture = GetInstance()->mMaterialTextureMap[tag].get();

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
		std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", is,failed to load";
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

// 深度テクスチャの生成
void TextureManager::CreateDepthTexture(DepthBuffer* depthBuffer, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// マップに格納
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<DepthTexture>())));

	DepthTexture* texture = dynamic_cast<DepthTexture*>(GetInstance()->mTextureMap[tag].get());

	// 深度テクスチャのバッファ生成
	texture->Create(depthBuffer);

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(texture->GetBufferResource());

	std::string log = "[Texture Create] DepthTexture, Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());
}

// ボリュームテクスチャの生成
void TextureManager::CreateVolumeTexture(const std::vector<std::string>& filePathes, const Vec3 size, const std::string tag)
{
	// 排他制御
	//std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// マップに格納
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<VolumeTexture>())));

	VolumeTexture* texture = dynamic_cast<VolumeTexture*>(GetInstance()->mTextureMap[tag].get());

	HRESULT result;
	std::vector<TexMetadata> metadatas(filePathes.size());
	std::vector<ScratchImage> scratchImgs(filePathes.size());

	for (uint32_t i = 0; i < filePathes.size(); i++)
	{
		std::string path = "Application/Resources/Texture/" + filePathes[i];
		std::wstring wfilePath(path.begin(), path.end());

		// WICを使用してテクスチャデータを読み込む
		result = LoadFromWICFile(
			wfilePath.c_str(),
			WIC_FLAGS_NONE,
			&metadatas[i], scratchImgs[i]);
	}

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex3D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			static_cast<uint64_t>(size.x),
			static_cast<uint32_t>(size.y),
			//static_cast<uint16_t>(size.z),
			1,
			1);
	// バッファ生成
	texture->Create(size, resourceDesc);

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(texture->GetBufferResource());

	// イメージデータを取得してサブリソースデータを設定
	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas(3);
	for (uint32_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// サブリソースデータにバッファを設定
		const Image* img = scratchImgs[i].GetImage(0, 0, 0);
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
		1,
		subResourcesDatas.data());

	// リソースの状態変更
	RenderBase::GetInstance()->TransitionBufferState(
		texture->GetBufferResource(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	std::string log = "[Texture Create] VolumeTexture, Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());
}

// アンロード
void TextureManager::UnLoadTexture(const std::string tag)
{
	auto it = GetInstance()->mTextureMap.find(tag);
	if (it == GetInstance()->mTextureMap.end())
	{
		return;
	}

	// SRVを上書きできる状態にする
	DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(
		GetInstance()->mTextureMap[tag]->GetBufferResource());

	// Mapから削除
	GetInstance()->mTextureMap.erase(tag);
}

// テクスチャーを取得する関数
Texture* TextureManager::GetTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// ログ
	std::string log = "Error";

	ITexture* iTexture = GetInstance()->mTextureMap[tag].get();
	if (iTexture == nullptr)
	{
		log = "[Texture Use] Tag : " + tag + ", does not exist";
	}
	else
	{
		log = "[Texture Use] Tag : " + tag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return dynamic_cast<Texture*>(iTexture);
}

// 深度テクスチャーを取得する関数
DepthTexture* TextureManager::GetDepthTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// ログ
	std::string log = "Error";

	ITexture* iTexture = GetInstance()->mTextureMap[tag].get();
	if (iTexture == nullptr)
	{
		log = "[DepthTexture Use] Tag : " + tag + ", does not exist";
	}
	else
	{
		log = "[DepthTexture Use] Tag : " + tag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return dynamic_cast<DepthTexture*>(iTexture);
}

// ボリュームテクスチャを取得する関数
VolumeTexture* TextureManager::GetVolumeTexture(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// ログ
	std::string log = "Error";

	ITexture* iTexture = GetInstance()->mTextureMap[tag].get();
	if (iTexture == nullptr)
	{
		log = "[VolumeTexture Use] Tag : " + tag + ", does not exist";
	}
	else
	{
		log = "[VolumeTexture Use] Tag : " + tag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return dynamic_cast<VolumeTexture*>(iTexture);
}


// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ縺ｮ蜿門ｾ・
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

// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ繧堤函謌舌＠繝槭ャ繝励↓譬ｼ邏阪☆繧・
RenderTexture* TextureManager::CreateRenderTexture(const Vec2 size, const uint32_t num, const std::string tag)
{
	// 謗剃ｻ門宛蠕｡
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	std::unique_ptr<RenderTexture> renderTex = std::make_unique<RenderTexture>();
	renderTex->size = size;
	renderTex->GetBufferResources()->resize(num);

	HRESULT result;
	RenderBase* renderBase = RenderBase::GetInstance();

	// 繝偵・繝苓ｨｭ螳・
	CD3DX12_HEAP_PROPERTIES heapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			(UINT64)size.x, (UINT)size.y,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_CLEAR_VALUE clearValue =
		CD3DX12_CLEAR_VALUE(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			RenderTexture::sClearColor);

	for (uint32_t i = 0; i < num; i++)
	{
		result = renderBase->GetDevice()->
			CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				IID_PPV_ARGS(&renderTex->GetBufferResources()->at(i).buffer));
		assert(SUCCEEDED(result));
	}

	for (uint32_t i = 0; i < num; i++)
	{
		// SRV菴懈・
		DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(&renderTex->GetBufferResources()->at(i));

		// RTV菴懈・
		DescriptorHeapManager::GetDescriptorHeap("RTV")->CreateRTV(&renderTex->GetBufferResources()->at(i));
	}

	// DSV菴懈・
	renderTex->depthBuffer.Create(size);
	DescriptorHeapManager::GetDescriptorHeap("DSV")->CreateDSV(renderTex->depthBuffer.GetBufferResource());

	renderTex->depthTexture = std::make_unique<Texture>();
	renderTex->depthTexture->GetBufferResource()->buffer = renderTex->depthBuffer.GetBufferResource()->buffer;
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(renderTex->depthTexture->GetBufferResource());

	GetInstance()->mRenderTextureMap.insert(std::make_pair(tag, std::move(renderTex)));

	std::string log = "[RenderTexture Create] Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());

	return GetInstance()->mRenderTextureMap[tag].get();
}

// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ縺ｮ繧｢繝ｳ繝ｭ繝ｼ繝蛾未謨ｰ
void TextureManager::UnLoadRenderTexture(const std::string tag)
{
	auto it = GetInstance()->mRenderTextureMap.find(tag);
	if (it == GetInstance()->mRenderTextureMap.end())
	{
		return;
	}


	for (uint32_t i = 0; i < GetInstance()->mRenderTextureMap[tag]->GetBufferResources()->size(); i++)
	{
		// 繝薙Η繝ｼ蜑企勁
		DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(
			&GetInstance()->mRenderTextureMap[tag]->GetBufferResources()->at(i));

		// 繝槭ャ繝励°繧牙炎髯､
		GetInstance()->mTextureMap.erase(tag);
	}
}

// 繝・け繧ｹ繝√Ε繝槭ャ繝励・蜿門ｾ・
std::unordered_map<std::string, std::unique_ptr<ITexture>>* TextureManager::GetTextureMap()
{
	return &GetInstance()->mTextureMap;
}

// 繝・け繧ｹ繝√Ε繝槭ャ繝励・蜿門ｾ・
std::unordered_map<std::string, std::unique_ptr<Texture>>* TextureManager::GetMaterialTextureMap()
{
	return &GetInstance()->mMaterialTextureMap;
}

// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝槭ャ繝励・蜿門ｾ・
std::unordered_map<std::string, std::unique_ptr<RenderTexture>>* TextureManager::GetRenderTextureMap()
{
	return &GetInstance()->mRenderTextureMap;
}

// 繝・け繧ｹ繝√Ε繝ｼ繝ｭ繝ｼ繝牙ｾ後・繧ｳ繝槭Φ繝峨Μ繧ｹ繝医・螳溯｡・
void TextureManager::ExcuteComandList()
{
	ID3D12GraphicsCommandList* iCommandList = RenderBase::GetInstance()->GetCommandList();

	iCommandList->Close();

	ID3D12CommandQueue* iCommandQueue = RenderBase::GetInstance()->GetCommandQueue();

	ID3D12CommandList* list[] = { iCommandList };
	iCommandQueue->ExecuteCommandLists(1, list);

	RenderBase::GetInstance()->PreIncrimentFenceValue();

	// 繧ｳ繝槭Φ繝峨・螳溯｡悟ｮ御ｺ・ｒ蠕・▽
	iCommandQueue->Signal(RenderBase::GetInstance()->GetFence(), RenderBase::GetInstance()->GetFenceValue());

	if (RenderBase::GetInstance()->GetFence()->GetCompletedValue() != RenderBase::GetInstance()->GetFenceValue())
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		RenderBase::GetInstance()->GetFence()->SetEventOnCompletion(RenderBase::GetInstance()->GetFenceValue(), event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	HRESULT result;

	// 繧ｭ繝･繝ｼ繧偵け繝ｪ繧｢
	result = RenderBase::GetInstance()->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
	// 蜀阪・繧ｳ繝槭Φ繝峨Μ繧ｹ繝医ｒ雋ｯ繧√ｋ貅門ｙ
	result = iCommandList->Reset(RenderBase::GetInstance()->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}

