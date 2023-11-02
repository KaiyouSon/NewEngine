#include "LoadManager.h"
#include "NewEngine.h"
#include "AssimpLoader.h"
#include "MotionManager.h"
#include "FieldDataManager.h"

///////////////////////////////////////////////////////////////
// --- タイトルシーンでのロード・アンロード ----------------///
///////////////////////////////////////////////////////////////
void LoadManager::TitleSceneLoad()
{
	TextureManager::LoadTexture("Particle/Particle1.png", "Particle1");
	TextureManager::LoadTexture("Title/GameTitle.png", "GameTitle");
	TextureManager::LoadTexture("Title/PressButton.png", "PressButton");
	TextureManager::LoadTexture("Title/PressButtonBack.png", "PressButtonBack");

	// ブルーム用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BloomHighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BloomGaussianBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Bloom");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BloomTarget");
}
void LoadManager::TitleSceneUnLoad()
{
	TextureManager::DestroyTexture("Particle1");
	TextureManager::DestroyTexture("GameTitle");
	TextureManager::DestroyTexture("PressButton");
	TextureManager::DestroyTexture("PressButtonBack");

	// ブルーム用
	TextureManager::DestroyRenderTexture("BloomHighLumi");
	TextureManager::DestroyRenderTexture("BloomGaussianBlur");
	TextureManager::DestroyRenderTexture("Bloom");
	TextureManager::DestroyRenderTexture("BloomTarget");
}

///////////////////////////////////////////////////////////////
// --- ゲームシーンでのロード・アンロード ------------------///
///////////////////////////////////////////////////////////////
void LoadManager::GameSceneLoad()
{
	// テクスチャ
	GameSceneTextureLoad();

	// モデル
	GameSceneModelLoad();
}
void LoadManager::GameSceneUnLoad()
{
	// テクスチャ
	GameSceneTextureUnLoad();

	// モデル
	GameSceneModelUnLoad();
}

// テクスチャ
void LoadManager::GameSceneTextureLoad()
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
	TextureManager::LoadTextureFromDDS("Text/ColonStr.dds", "ColonStr");
	TextureManager::LoadTextureFromDDS("Text/Negotiation/ReadMessageStr.dds", "ReadMessageStr");
	TextureManager::LoadTextureFromDDS("Text/Negotiation/RestInLightStr.dds", "RestInLightStr");
	TextureManager::LoadTextureFromDDS("Text/Negotiation/OpenStr.dds", "OpenStr");
	TextureManager::LoadTextureFromDDS("Text/ExitStr.dds", "ExitStr");
	TextureManager::LoadTextureFromDDS("Text/Operation/Operation1Text.dds", "Operation1Text");
	TextureManager::LoadTextureFromDDS("Text/Operation/Operation2Text.dds", "Operation2Text");
	TextureManager::LoadTextureFromDDS("Text/Operation/Operation3Text.dds", "Operation3Text");
	TextureManager::LoadTextureFromDDS("Text/Operation/Operation4Text.dds", "Operation4Text");
	TextureManager::LoadTextureFromDDS("Text/Operation/Operation5Text.dds", "Operation5Text");
	TextureManager::LoadTextureFromDDS("Text/Menu/BackToTitleStr.dds", "BackToTitleStr");
	TextureManager::LoadTextureFromDDS("Text/Menu/CloseGameStr.dds", "CloseGameStr");
	TextureManager::LoadTextureFromDDS("Text/Result/EnemyFelledStr.dds", "EnemyFelledStr");
	TextureManager::LoadTextureFromDDS("Text/Result/YouDiedStr.dds", "YouDiedStr");
	TextureManager::LoadTextureFromDDS("Text/RespawnPoint/DecisionCloseStr.dds", "DecisionCloseStr");
	TextureManager::LoadTextureFromDDS("Text/NumberSheets.dds", "NumberSheets");

	// パーティクル
	TextureManager::LoadTextureFromDDS("Particle/Particle1.dds", "Particle1");
	TextureManager::LoadTextureFromDDS("Particle/Particle2.dds", "Particle2");
	TextureManager::LoadTextureFromDDS("Particle/Line.dds", "Line");
	TextureManager::LoadTextureFromDDS("Trajectory.dds", "Trajectory");

	// 草
	TextureManager::LoadTextureFromDDS("Grass/Weed.dds", "Weed");
	TextureManager::LoadTextureFromDDS("Branch.dds", "Branch");

	// 天球
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Vignette");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "VolumetricFog");

	// ブルーム用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BloomHighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BloomGaussianBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Bloom");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BloomTarget");
}
void LoadManager::GameSceneTextureUnLoad()
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
	TextureManager::DestroyTexture("Trajectory");

	// 草
	TextureManager::DestroyTexture("Weed");
	TextureManager::DestroyTexture("Branch");

	// 天球
	TextureManager::DestroyTexture("Vignette");
	TextureManager::DestroyTexture("VolumetricFog");

	// エフェクトブルーム用
	TextureManager::DestroyRenderTexture("BloomHighLumi");
	TextureManager::DestroyRenderTexture("BloomGaussianBlur");
	TextureManager::DestroyRenderTexture("Bloom");
	TextureManager::DestroyRenderTexture("BloomTarget");
}

// モデル
void LoadManager::GameSceneModelLoad()
{
	// フィールド
	ModelManager::LoadObjModel("Ground", "Ground");
	ModelManager::LoadObjModel("Skydome", "Skydome", true);
	ModelManager::LoadObjModel("Cloud", "Cloud");
	ModelManager::LoadObjModel("Tree", "Tree", true);
	ModelManager::LoadObjModel("Branch", "Branch");
	ModelManager::LoadObjModel("CoffinTop", "CoffinTop");
	ModelManager::LoadObjModel("CoffinBottom", "CoffinBottom");
	ModelManager::LoadObjModel("SkyIsland/SkyIsland1", "SkyIsland1", true);
	ModelManager::LoadObjModel("SkyIsland/SkyIsland2", "SkyIsland2", true);
	ModelManager::LoadObjModel("Wall/MainWall", "MainWall");
	ModelManager::LoadObjModel("Wall/Wall1", "Wall1");
	ModelManager::LoadObjModel("Wall/Wall2", "Wall2");
	ModelManager::LoadObjModel("Wall/WallGate", "WallGate");

	// キャラクター(プレイヤー)
	ModelManager::LoadObjModel("HumanoidBody/Body", "Body");
	ModelManager::LoadObjModel("HumanoidBody/Head", "Head");
	ModelManager::LoadObjModel("HumanoidBody/Limbs", "Limbs");

	// キャラクター(ボス)
	ModelManager::LoadObjModel("Boss/BossHead", "BossHead");
	ModelManager::LoadObjModel("Boss/BossBody", "BossBody");
	ModelManager::LoadObjModel("Boss/BossLeftArm", "BossLeftArm");
	ModelManager::LoadObjModel("Boss/BossRightArm", "BossRightArm");

	// 武器
	ModelManager::LoadObjModel("Weapon/Club", "Club", true);
	ModelManager::LoadObjModel("Weapon/Sword", "Sword", true);

}
void LoadManager::GameSceneModelUnLoad()
{
	// フィールド
	ModelManager::DestroyModel("Ground");
	ModelManager::DestroyModel("Skydome");
	ModelManager::DestroyModel("Cloud");
	ModelManager::DestroyModel("Tree");
	ModelManager::DestroyModel("Branch");
	ModelManager::DestroyModel("CoffinTop");
	ModelManager::DestroyModel("CoffinBottom");
	ModelManager::DestroyModel("SkyIsland1");
	ModelManager::DestroyModel("SkyIsland2");
	ModelManager::DestroyModel("MainWall");
	ModelManager::DestroyModel("Wall1");
	ModelManager::DestroyModel("Wall2");
	ModelManager::DestroyModel("WallGate");

	// キャラクター(プレイヤー)
	ModelManager::DestroyModel("Body");
	ModelManager::DestroyModel("Head");
	ModelManager::DestroyModel("Limbs");

	// キャラクター(ボス)
	ModelManager::DestroyModel("BossHead");
	ModelManager::DestroyModel("BossBody");
	ModelManager::DestroyModel("BossLeftArm");
	ModelManager::DestroyModel("BossRightArm");

	// 武器
	ModelManager::DestroyModel("Club");
	ModelManager::DestroyModel("Sword");
}

///////////////////////////////////////////////////////////////
// --- ゲーム起動時 ----------------------------------------///
///////////////////////////////////////////////////////////////
bool LoadManager::ModelLoad()
{
	ModelManager::LoadObjModel("Cube", "Cube");
	ModelManager::LoadObjModel("Sphere", "Sphere");

	ModelManager::LoadObjModel("Plane", "Plane");

	ModelManager::LoadObjModel("MessageSign", "MessageSign");

	ModelManager::LoadObjModel("Capsule", "Capsule");
	ModelManager::LoadObjModel("MainGateWall", "MainGateWall");

	// 非同期終わったよ～
	return true;
}
bool LoadManager::TextureLoad()
{
	// タイトルロゴ
	TextureManager::LoadTextureFromDDS("Title/TitleLogo.dds", "TitleLogo");

	// ディゾルブ
	TextureManager::LoadTextureFromDDS("DissolveTexture.dds", "DissolveTexture");

	// ブラー
	TextureManager::LoadTextureFromDDS("Noice/BlurNoice.dds", "BlurNoice");
	TextureManager::LoadTextureFromDDS("Noice/ScreenNoice.dds", "ScreenNoice");

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
