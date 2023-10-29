#include "LoadManager.h"
#include "NewEngine.h"
#include "AssimpLoader.h"
#include "MotionManager.h"
#include "FieldDataManager.h"

// タイトルシーンでのロード・アンロード
void LoadManager::TitleSceneLoad()
{
	TextureManager::LoadTexture("Particle/Particle1.png", "Particle1");
	TextureManager::LoadTexture("Title/GameTitle.png", "GameTitle");
	TextureManager::LoadTexture("Title/PressButton.png", "PressButton");
	TextureManager::LoadTexture("Title/PressButtonBack.png", "PressButtonBack");

	// ブルーム用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "HighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "GaussianBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "EffectBloom");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "EffectBloomTarget");
}
void LoadManager::TitleSceneUnLoad()
{
	TextureManager::DestroyTexture("Particle1");
	TextureManager::DestroyTexture("GameTitle");
	TextureManager::DestroyTexture("PressButton");
	TextureManager::DestroyTexture("PressButtonBack");

	// ブルーム用
	TextureManager::DestroyRenderTexture("HighLumi");
	TextureManager::DestroyRenderTexture("GaussianBlur");
	TextureManager::DestroyRenderTexture("EffectBloom");
	TextureManager::DestroyRenderTexture("EffectBloomTarget");
}

// ゲームシーンでのロード・アンロード
void LoadManager::GameSceneLoad()
{
	// UI
	TextureManager::LoadTexture("UI/Gauge.png", "Gauge");
	TextureManager::LoadTexture("UI/Buttons.png", "Buttons");
	TextureManager::LoadTexture("UI/Negotiation/NegotiationBack.png", "NegotiationBack");
	TextureManager::LoadTexture("UI/MessageSign/MessageBack.png", "MessageBack");
	TextureManager::LoadTexture("UI/MessageSign/MessageSignUI.png", "MessageSignUI");
	TextureManager::LoadTexture("UI/ItemBoxFrame.png", "ItemBoxFrame");
	TextureManager::LoadTexture("UI/ItemBoxLight.png", "ItemBoxLight");
	TextureManager::LoadTexture("UI/ItemUI/BottleUI.png", "BottleUI");
	TextureManager::LoadTexture("UI/ItemUI/ClubUI.png", "ClubUI");
	TextureManager::LoadTexture("UI/Menu/TempMenuBack.png", "MenuBack");
	TextureManager::LoadTexture("UI/Menu/MenuTextFrame.png", "MenuTextFrame");
	TextureManager::LoadTexture("UI/Menu/MenuTextLight.png", "MenuTextLight");
	TextureManager::LoadTexture("UI/Result/ResultBack.png", "ResultBack");
	TextureManager::LoadTexture("UI/Operation/OperationBack.png", "OperationBack");

	// テキスト
	TextureManager::LoadTexture("Text/ColonStr.png", "ColonStr");
	TextureManager::LoadTexture("Text/Negotiation/ReadMessageStr.png", "ReadMessageStr");
	TextureManager::LoadTexture("Text/Negotiation/RestInLightStr.png", "RestInLightStr");
	TextureManager::LoadTexture("Text/Negotiation/OpenStr.png", "OpenStr");
	TextureManager::LoadTexture("Text/ExitStr.png", "ExitStr");
	TextureManager::LoadTexture("Text/Operation/Operation1Text.png", "Operation1Text");
	TextureManager::LoadTexture("Text/Operation/Operation2Text.png", "Operation2Text");
	TextureManager::LoadTexture("Text/Operation/Operation3Text.png", "Operation3Text");
	TextureManager::LoadTexture("Text/Operation/Operation4Text.png", "Operation4Text");
	TextureManager::LoadTexture("Text/Operation/Operation5Text.png", "Operation5Text");
	TextureManager::LoadTexture("Text/Menu/BackToTitleStr.png", "BackToTitleStr");
	TextureManager::LoadTexture("Text/Menu/CloseGameStr.png", "CloseGameStr");
	TextureManager::LoadTexture("Text/Result/EnemyFelledStr.png", "EnemyFelledStr");
	TextureManager::LoadTexture("Text/Result/YouDiedStr.png", "YouDiedStr");
	TextureManager::LoadTexture("Text/RespawnPoint/DecisionCloseStr.png", "DecisionCloseStr");
	TextureManager::LoadTexture("Text/NumberSheets.png", "NumberSheets");

	// パーティクル
	TextureManager::LoadTexture("Particle/Particle1.png", "Particle1");
	TextureManager::LoadTexture("Particle/Particle2.png", "Particle2");
	TextureManager::LoadTexture("Particle/Line.png", "Line");

	// 草
	TextureManager::LoadTexture("Grass/Weed.png", "Weed");
	TextureManager::LoadTexture("Branch.png", "Branch");

	// 天球
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Vignette");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "VolumetricFog");

	// ブルーム用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "HighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "GaussianBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "EffectBloom");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "EffectBloomTarget");
}
void LoadManager::GameSceneUnLoad()
{
	// UI
	TextureManager::DestroyTexture("Gauge");
	TextureManager::DestroyTexture("Buttons");
	TextureManager::DestroyTexture("NegotiationBack");
	TextureManager::DestroyTexture("MessageBack");
	TextureManager::DestroyTexture("MessageSignUI");
	TextureManager::DestroyTexture("ItemBoxFrame");
	TextureManager::DestroyTexture("ItemBoxLight");
	TextureManager::DestroyTexture("BottleUI");
	TextureManager::DestroyTexture("ClubUI");
	TextureManager::DestroyTexture("MenuBack");
	TextureManager::DestroyTexture("MenuTextFrame");
	TextureManager::DestroyTexture("MenuTextLight");
	TextureManager::DestroyTexture("ResultBack");
	TextureManager::DestroyTexture("RespawnBack");

	// テキスト
	TextureManager::DestroyTexture("ColonStr");
	TextureManager::DestroyTexture("ReadMessageStr");
	TextureManager::DestroyTexture("RestInLightStr");
	TextureManager::DestroyTexture("OpenStr");
	TextureManager::DestroyTexture("ExitStr");
	TextureManager::DestroyTexture("Operation1Text");
	TextureManager::DestroyTexture("Operation2Text");
	TextureManager::DestroyTexture("Operation3Text");
	TextureManager::DestroyTexture("Operation4Text");
	TextureManager::DestroyTexture("Operation5Text");
	TextureManager::DestroyTexture("BackToTitleStr");
	TextureManager::DestroyTexture("CloseGameStr");
	TextureManager::DestroyTexture("EnemyFelledStr");
	TextureManager::DestroyTexture("YouDiedStr");
	TextureManager::DestroyTexture("DecisionCloseStr");
	TextureManager::DestroyTexture("NumberSheets");

	// パーティクル
	TextureManager::DestroyTexture("Particle1");
	TextureManager::DestroyTexture("Particle2");
	TextureManager::DestroyTexture("Line");

	// 草
	TextureManager::DestroyTexture("Weed");
	TextureManager::DestroyTexture("Branch");

	// 天球
	TextureManager::DestroyTexture("Vignette");
	TextureManager::DestroyTexture("VolumetricFog");

	// ブルーム用
	TextureManager::DestroyTexture("HighLumi");
	TextureManager::DestroyTexture("GaussianBlur");
	TextureManager::DestroyRenderTexture("EffectBloom");
	TextureManager::DestroyRenderTexture("EffectBloomTarget");
}

bool LoadManager::ModelLoad()
{
	ModelManager::LoadObjModel("Block1", "Block1");
	ModelManager::LoadFbxModel("boneTest", "BoneTest");
	ModelManager::LoadObjModel("Cube", "Cube");
	ModelManager::LoadObjModel("Sphere", "Sphere");

	ModelManager::LoadObjModel("Plane", "Plane");

	ModelManager::LoadObjModel("Ground", "Ground");
	ModelManager::LoadObjModel("Skydome", "Skydome", true);
	ModelManager::LoadObjModel("Cloud", "Cloud");
	ModelManager::LoadObjModel("Tree", "Tree", true);
	ModelManager::LoadObjModel("Branch", "Branch");

	ModelManager::LoadObjModel("MessageSign", "MessageSign");

	ModelManager::LoadObjModel("HumanoidBody/Body", "Body");
	ModelManager::LoadObjModel("HumanoidBody/Head", "Head");
	ModelManager::LoadObjModel("HumanoidBody/Limbs", "Limbs");

	ModelManager::LoadObjModel("Weapon/Club", "Club", true);
	ModelManager::LoadObjModel("Weapon/Sword", "Sword", true);

	ModelManager::LoadObjModel("CoffinTop", "CoffinTop");
	ModelManager::LoadObjModel("CoffinBottom", "CoffinBottom");
	ModelManager::LoadObjModel("SkyIsland/SkyIsland1", "SkyIsland1", true);
	ModelManager::LoadObjModel("SkyIsland/SkyIsland2", "SkyIsland2", true);
	ModelManager::LoadObjModel("Wall/MainWall", "MainWall");
	ModelManager::LoadObjModel("Wall/Wall1", "Wall1");
	ModelManager::LoadObjModel("Wall/Wall2", "Wall2");
	ModelManager::LoadObjModel("Wall/WallGate", "WallGate");

	ModelManager::LoadObjModel("Capsule", "Capsule");
	ModelManager::LoadObjModel("MainGateWall", "MainGateWall");

	ModelManager::LoadObjModel("Boss/BossHead", "BossHead");
	ModelManager::LoadObjModel("Boss/BossBody", "BossBody");
	ModelManager::LoadObjModel("Boss/BossLeftArm", "BossLeftArm");
	ModelManager::LoadObjModel("Boss/BossRightArm", "BossRightArm");

	// 非同期終わったよ～
	return true;
}

bool LoadManager::TextureLoad()
{
	// タイトルロゴ
	TextureManager::LoadTexture("Title/TitleLogo.png", "TitleLogo");

	// ディゾルブ
	TextureManager::LoadTexture("DissolveTexture.png", "DissolveTexture");

	// ブラー
	TextureManager::LoadTexture("Noice/BlurNoice.png", "BlurNoice");
	TextureManager::LoadTexture("Noice/ScreenNoice.png", "ScreenNoice");

	// シャドウマップ
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 8, 1, "ShadowMap");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 8, 1, "ShadowMapBlur");

	// 現在のシーン
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "CurrentScene");

	// ボリュームテクスチャの作成
	std::vector<Texture*> texs;
	for (uint32_t i = 0; i < 16; i++)
	{
		// ボリュームノイズのロード
		std::string index = std::to_string(i);
		std::string path = "Noice/VolumeNoice/VolumeNoice" + index + ".png";
		std::string tag = "Noice" + index;
		TextureManager::LoadTexture(path, tag);
		texs.push_back(TextureManager::GetTexture(tag));
	}
	TextureManager::CreateVolumeTexture(texs, "VolumeTexture");

	// 非同期終わったよ～
	return true;
}

bool LoadManager::SoundLoad()
{
	// BGM
	SoundManager::LoadSound("BGM/TitleBGM.wav", "TitleBGM");
	SoundManager::LoadSound("BGM/BattleBGM.wav", "BattleBGM");

	// SE
	SoundManager::LoadSound("SE/HitSE.wav", "HitSE");
	SoundManager::LoadSound("SE/WeakAttackSE.wav", "WeakAttackSE");
	SoundManager::LoadSound("SE/SelectSE.wav", "SelectSE");
	SoundManager::LoadSound("SE/BackstepSE.wav", "BackstepSE");
	SoundManager::LoadSound("SE/RollSE.wav", "RollSE");
	SoundManager::LoadSound("SE/RecoverySE.wav", "RecoverySE");

	SoundManager::LoadSound("SE/BossAttackSE.wav", "BossAttackSE");

	// 非同期終わったよ～
	return true;
}

bool LoadManager::MotionLoad()
{
	MotionManager::Load("Player/BackstepMotion", "Backstep");
	MotionManager::Load("Player/RollMotion", "Roll");
	MotionManager::Load("Player/DrinkMotion", "Drink");
	MotionManager::Load("Player/OpenGateMotion", "OpenGate");
	MotionManager::Load("Weapon/Mallet/WeakAttackMotion1", "MalletWeakAttack1");
	MotionManager::Load("Weapon/Mallet/WeakAttackMotion2", "MalletWeakAttack2");
	MotionManager::Load("Weapon/Mallet/WeakAttackMotion3", "MalletWeakAttack3");
	MotionManager::Load("Weapon/Mallet/HeavyAttackMotion", "MalletHeavyAttack");
	MotionManager::Load("Weapon/Mallet/BackAttackMotion", "MalletBackAttack");
	MotionManager::Load("Weapon/Mallet/RollAttackMotion", "MalletRollAttack");

	MotionManager::Load("Boss/GrabAttack", "BossGrabAttack");
	MotionManager::Load("Boss/Attack2", "BossAttack2");
	MotionManager::Load("Boss/Attack3", "BossAttack3");

	// 非同期終わったよ～
	return true;
}

LoadManager::LoadManager() : mIsLoaded(false)
{
}
void LoadManager::Load()
{
	// モデルで使うから先に作っとく
	TextureManager::CreateColorTexture(Color::white, "White");

	std::future<bool> textureFtr = std::async(std::launch::async, [this] { return TextureLoad(); });
	std::future<bool> modelFtr = std::async(std::launch::async, [this] { return ModelLoad(); });
	std::future<bool> soundFtr = std::async(std::launch::async, [this] { return SoundLoad(); });
	std::future<bool> motionFtr = std::async(std::launch::async, [this] { return MotionLoad(); });

	//	全ロード終わったら
	if (textureFtr.get() == true &&
		modelFtr.get() == true &&
		soundFtr.get() == true &&
		motionFtr.get() == true)
	{
		mIsLoaded = true;

		// コマンド実行
		TextureManager::ExcuteComandList();
	}
}
