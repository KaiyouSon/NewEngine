#include "LoadManager.h"
#include "NewEngine.h"
#include "AssimpLoader.h"

bool LoadManager::ModelLoad()
{
	ModelManager::LoadObjModel("SkyDome", "SkyDome");
	ModelManager::LoadObjModel("Block1", "Block1");
	ModelManager::LoadFbxModel("boneTest", "BoneTest");
	ModelManager::LoadObjModel("Cube", "Cube");

	ModelManager::LoadObjModel("HumanoidBody/Body", "Body");
	ModelManager::LoadObjModel("HumanoidBody/Head", "Head");
	ModelManager::LoadObjModel("HumanoidBody/Limbs", "Limbs");

	// 処理が終わったのを教えるため、必ずtrueを返す
	return true;
}

bool LoadManager::TextureLoad()
{
	TextureManager::CreateTexture(Color::white, "White");

	TextureManager::LoadTexture("pic.png", "pic");
	TextureManager::LoadTexture("BackGround.png", "BackGround");
	TextureManager::LoadTexture("Gauge.png", "Gauge");

	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 2, "Task");

	// 高輝度抽出
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "HighLumi");

	// 背景
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BackGround");

	// ドッドフィルター
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "DotFilter");

	// ビネット
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Vignette");

	// ブルーム
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BGaussainBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Bloom");

	// ガウシアンブラー
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "GaussianBlur");

	// グレア
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "GLineBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Glare1");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Glare2");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Glare3");

	// 被写界深度
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "DOF");

	// ラジアルブラー
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "RadialBlur");


	TextureManager::CreateDepthTexture(Vec2(1920, 1080));

	// 処理が終わったのを教えるため、必ずtrueを返す
	return true;
}

bool LoadManager::SoundLoad()
{
	//SoundManager::LoadSound("GameBGM.wav", "GameBGM");

	// 処理が終わったのを教えるため、必ずtrueを返す
	return true;
}

LoadManager::LoadManager() : isLoaded(false)
{
}

void LoadManager::Load()
{
	// ロード前
	TextureManager::CreateTexture(Color::white, "White");

	// 非同期
	std::future<bool> textureFtr = std::async(std::launch::async, [this] { return TextureLoad(); });
	std::future<bool> modelFtr = std::async(std::launch::async, [this] { return ModelLoad(); });
	std::future<bool> soundFtr = std::async(std::launch::async, [this] { return SoundLoad(); });

	// ロード完了
	if (textureFtr.get() == true &&	// テクスチャー
		modelFtr.get() == true &&	// モデル
		soundFtr.get() == true)		// サウンド
	{
		isLoaded = true;
		// コマンド実行
		TextureManager::ExcuteComandList();
	}

}