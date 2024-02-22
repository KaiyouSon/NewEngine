#include "AssetsManager.h"
namespace fs = std::filesystem;
using namespace DirectX;

void AssetsManager::SaveToJson(const std::string& sceneName)
{
	nlohmann::json textureAssets;
	for (uint32_t i = 0; i < mTextureMapArrays.size(); i++)
	{
		for (const auto& [tag, tex] : mTextureMapArrays[i])
		{
			nlohmann::json assetsData;
			assetsData["path"] = tex->GetPath();

			textureAssets.push_back(assetsData);
		}
	}

	nlohmann::json data;
	data["textures"] = textureAssets;

	std::ofstream file(EngineDataDirectory + "Assets/" + sceneName + "Assets.json");
	file << std::setw(4) << data << std::endl;
}

void AssetsManager::LoadToJson(const std::string& sceneName)
{
	std::string path = EngineDataDirectory + "Assets/" + sceneName + "Assets.json";

	// ファイルを開く
	std::ifstream file(path);
	// ファイルが開けない場合はアサーションエラー
	if (file.fail())
	{
		assert(0);
	}

	// JSONをデシリアライズ
	nlohmann::json deserialized;
	file >> deserialized;

	// "objects"フィールドの各オブジェクトを処理
	for (nlohmann::json& tex : deserialized["textures"])
	{
		std::string texPath = tex["path"];
		LoadTexture(texPath);
	}

	file.close();

	//LoadTexture(AppTextureDirectory + "Grass/Weed.png");
}

void AssetsManager::Load()
{

	//mTextureManager.LoadTexture()
}

void AssetsManager::UnLoad()
{
}

void AssetsManager::LoadTexture(const std::string& path)
{
	// 排他制御
	//std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	fs::path fspath = path;
	std::string tag = fspath.filename().string();

	// 拡張子を含む最後の'.'以降を削除
	uint32_t pos = (uint32_t)tag.find_last_of('.');
	if (pos != std::string::npos)
	{
		tag = tag.substr(0, pos);
	}

	// マップに格納
	std::unique_ptr<ITexture> itex = std::make_unique<Texture>(tag, path);
	std::pair pair = std::make_pair(tag, std::move(itex));
	mTextureMapArrays[(uint32_t)TextureType::Default].insert(std::move(pair));

	// キャストする
	Texture* tex = dynamic_cast<Texture*>(mTextureMapArrays[(uint32_t)TextureType::Default][tag].get());

	TexMetadata metadata = TexMetadata();
	ScratchImage scratchImg = ScratchImage();

	// ロード
	if (fspath.extension() == ".png")
	{
		std::wstring wfilePath(path.begin(), path.end());

		// WICを使用してテクスチャデータを読み込む
		HRESULT result = LoadFromWICFile(
			wfilePath.c_str(),
			WIC_FLAGS_NONE,
			&metadata, scratchImg);

		if (result != S_OK)
		{
			std::string log = "[Texture Load] FilePath : " + path + ", is,failed to load";
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
	tex->Create(resourceDesc, static_cast<uint32_t>(metadata.mipLevels));

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(tex->GetBufferResource());

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
		tex->GetBufferResource()->buffer.Get(),
		tex->GetUploadBuffer()->GetBufferResource()->buffer.Get(),
		0,
		0,
		static_cast<uint32_t>(metadata.mipLevels),
		subResourcesDatas.data());

	// リソースの状態変更
	RenderBase::GetInstance()->TransitionBufferState(
		tex->GetBufferResource(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	// 使い終わったならテクスチャにmove
	tex->SetScratchImage(&scratchImg);

	//// Log出力
	//std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", was loaded successfully";
	//OutputDebugLog(log.c_str());
}

void LoadTexture(const std::string& path)
{
	SceneManager::GetInstance()->mCurrentScene->GetAssetsManager()->LoadTexture(path);
}
