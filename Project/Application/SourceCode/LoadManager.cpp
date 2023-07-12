#include "LoadManager.h"
#include "NewEngine.h"
#include "AssimpLoader.h"
#include "MotionManager.h"

bool LoadManager::ModelLoad()
{
	ModelManager::LoadObjModel("SkyDome", "SkyDome");
	ModelManager::LoadObjModel("Block1", "Block1");
	ModelManager::LoadFbxModel("boneTest", "BoneTest");
	ModelManager::LoadObjModel("Cube", "Cube");
	ModelManager::LoadObjModel("Sphere", "Sphere");

	ModelManager::LoadObjModel("Ground", "Ground");

	ModelManager::LoadObjModel("MessageSign", "MessageSign");

	ModelManager::LoadObjModel("HumanoidBody/Body", "Body");
	ModelManager::LoadObjModel("HumanoidBody/Head", "Head");
	ModelManager::LoadObjModel("HumanoidBody/Limbs", "Limbs");

	ModelManager::LoadObjModel("Weapon/Club", "Club");
	ModelManager::LoadObjModel("Capsule", "Capsule");
	ModelManager::LoadObjModel("Collider/CapsuleCollider", "CapsuleCollider");

	// 処理が終わったのを教えるため、必ずtrueを返す
	return true;
}

bool LoadManager::TextureLoad()
{
	TextureManager::CreateTexture(Color::white, "White");
	TextureManager::CreateDepthTexture(Vec2(1920, 1080));

	// タイトル
	TextureManager::LoadTexture("Title/GameTitle.png", "GameTitle");
	TextureManager::LoadTexture("Title/PressButton.png", "PressButton");
	TextureManager::LoadTexture("Title/PressButtonBack.png", "PressButtonBack");

	// UI
	TextureManager::LoadTexture("UI/Gauge.png", "Gauge");
	TextureManager::LoadTexture("UI/Buttons.png", "Buttons");
	TextureManager::LoadTexture("UI/Negotiation/NegotiationBackFrame.png", "NegotiationBackFrame");

	// テキスト
	TextureManager::LoadTexture("Text/ColonStr.png", "ColonStr");
	TextureManager::LoadTexture("Text/ReadMessageStr.png", "ReadMessageStr");

	// レンダーテクスチャー

	// 現在のシーン描画情報
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "CurrentScene");

	// ブルーム
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "HighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BGaussainBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Bloom");


	// 処理が終わったのを教えるため、必ずtrueを返す
	return true;
}

bool LoadManager::SoundLoad()
{
	//SoundManager::LoadSound("GameBGM.wav", "GameBGM");

	// 処理が終わったのを教えるため、必ずtrueを返す
	return true;
}

bool LoadManager::MotionLoad()
{
	MotionManager::Load("BackstepMotion", "Backstep");
	MotionManager::Load("RollMotion", "Roll");
	MotionManager::Load("Weapon/Mallet/WeakAttackMotion1", "MalletWeakAttack1");
	MotionManager::Load("Weapon/Mallet/WeakAttackMotion2", "MalletWeakAttack2");
	MotionManager::Load("Weapon/Mallet/WeakAttackMotion3", "MalletWeakAttack3");
	MotionManager::Load("Weapon/Mallet/HeavyAttackMotion", "MalletHeavyAttack");
	MotionManager::Load("Weapon/Mallet/BackAttackMotion", "MalletBackAttack");
	MotionManager::Load("Weapon/Mallet/RollAttackMotion", "MalletRollAttack");

	return true;
}

LoadManager::LoadManager() : isLoaded(false)
{
}

void LoadManager::Load()
{
	// 非同期
	std::future<bool> textureFtr = std::async(std::launch::async, [this] { return TextureLoad(); });
	std::future<bool> modelFtr = std::async(std::launch::async, [this] { return ModelLoad(); });
	std::future<bool> soundFtr = std::async(std::launch::async, [this] { return SoundLoad(); });
	std::future<bool> motionFtr = std::async(std::launch::async, [this] { return MotionLoad(); });

	// ロード完了
	if (textureFtr.get() == true &&	// テクスチャー
		modelFtr.get() == true &&	// モデル
		soundFtr.get() == true &&
		motionFtr.get() == true)	// サウンド
	{
		isLoaded = true;
		// コマンド実行
		TextureManager::ExcuteComandList();
	}

}