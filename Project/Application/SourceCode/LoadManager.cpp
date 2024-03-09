#include "LoadManager.h"
#include "NewEngine.h"
#include "AssimpLoader.h"
#include "MotionManager.h"
#include "ShaderManager.h"
#include "FieldDataManager.h"

///////////////////////////////////////////////////////////////
// --- タイトルシーンでのロード・アンロード ----------------///
///////////////////////////////////////////////////////////////
void LoadManager::TitleSceneLoad()
{
	TextureManager::LoadTexture("Particle/Particle1.dds", "Particle1");
	TextureManager::LoadTexture("Title/GameTitle.dds", "GameTitle");
	TextureManager::LoadTexture("Title/PressButton.dds", "PressButton");
	TextureManager::LoadTexture("Title/PressButtonBack.dds", "PressButtonBack");

	// ブルーム用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "BloomHighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) / 2, "BloomGaussianBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) / 4, "BloomGaussianBlurHalf");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "Bloom");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "BloomTarget");
}
void LoadManager::TitleSceneUnLoad()
{
	//TextureManager::DestroyTexture("Particle1");
	//TextureManager::DestroyTexture("GameTitle");
	//TextureManager::DestroyTexture("PressButton");
	//TextureManager::DestroyTexture("PressButtonBack");

	//// ブルーム用
	//TextureManager::DestroyRenderTexture("BloomHighLumi");
	//TextureManager::DestroyRenderTexture("BloomGaussianBlur");
	//TextureManager::DestroyRenderTexture("BloomGaussianBlurHalf");
	//TextureManager::DestroyRenderTexture("Bloom");
	//TextureManager::DestroyRenderTexture("BloomTarget");
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
	TextureManager::LoadTexture("UI/Gauge.dds", "Gauge");
	TextureManager::LoadTexture("UI/Buttons.dds", "Buttons");
	TextureManager::LoadTexture("UI/Negotiation/NegotiationBack.dds", "NegotiationBack");
	TextureManager::LoadTexture("UI/ItemBoxFrame.dds", "ItemBoxFrame");
	TextureManager::LoadTexture("UI/ItemBoxLight.dds", "ItemBoxLight");
	TextureManager::LoadTexture("UI/ItemUI/BottleUI.dds", "BottleUI");
	TextureManager::LoadTexture("UI/ItemUI/ClubUI.dds", "ClubUI");
	TextureManager::LoadTexture("UI/Menu/MenuBack.dds", "MenuBack");
	TextureManager::LoadTexture("UI/Menu/MenuTextFrame.dds", "MenuTextFrame");
	TextureManager::LoadTexture("UI/Menu/MenuTextLight.dds", "MenuTextLight");
	TextureManager::LoadTexture("UI/Result/ResultBack.dds", "ResultBack");
	TextureManager::LoadTexture("UI/Operation/OperationBack.dds", "OperationBack");

	// テキスト
	TextureManager::LoadTexture("Text/ColonStr.dds", "ColonStr");
	TextureManager::LoadTexture("Text/Negotiation/RestInLightStr.dds", "RestInLightStr");
	TextureManager::LoadTexture("Text/Negotiation/OpenStr.dds", "OpenStr");

	TextureManager::LoadTexture("Text/Operation/Operation1Text.dds", "Operation1Text");
	TextureManager::LoadTexture("Text/Operation/Operation2Text.dds", "Operation2Text");
	TextureManager::LoadTexture("Text/Operation/Operation3Text.dds", "Operation3Text");
	TextureManager::LoadTexture("Text/Operation/Operation4Text.dds", "Operation4Text");
	TextureManager::LoadTexture("Text/Operation/Operation5Text.dds", "Operation5Text");
	TextureManager::LoadTexture("Text/Menu/BackToTitleStr.dds", "BackToTitleStr");
	TextureManager::LoadTexture("Text/Menu/CloseGameStr.dds", "CloseGameStr");
	TextureManager::LoadTexture("Text/Result/EnemyFelledStr.dds", "EnemyFelledStr");
	TextureManager::LoadTexture("Text/Result/YouDiedStr.dds", "YouDiedStr");
	TextureManager::LoadTexture("Text/RespawnPoint/DecisionCloseStr.dds", "DecisionCloseStr");
	TextureManager::LoadTexture("Text/NumberSheets.dds", "NumberSheets");

	// パーティクル
	TextureManager::LoadTexture("Particle/Particle1.dds", "Particle1");
	TextureManager::LoadTexture("Particle/Particle2.dds", "Particle2");
	TextureManager::LoadTexture("Particle/Line.dds", "Line");
	TextureManager::LoadTexture("Particle/SmokeParticle.dds", "SmokeParticle");
	TextureManager::LoadTexture("Trajectory.dds", "Trajectory");

	// 草
	TextureManager::LoadTexture("Grass/Weed.dds", "Weed");
	TextureManager::LoadTexture("Branch.dds", "Branch");

	// ラジアルブラー用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) / 4, "RadialBlurTarget");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "RadialBlurFinish");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "RadialBlurScene");

	// ブルーム用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "BloomHighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) / 2, "BloomGaussianBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) / 4, "BloomGaussianBlurHalf");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "Bloom");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "BloomTarget");

	// レンズフレア用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "LensFlareTarget");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "LensFlareMask");

	// トーンマッピング用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "ToneMapping");

	// ビネット用
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "VignetteTarget");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "VignetteMask");

	// ボリュームテクスチャ
	TextureManager::LoadVolumeTexture("Noice/VolumeNoice/VolumeNoice0.dds", "VolumeTexture0");
	TextureManager::LoadVolumeTexture("Noice/VolumeNoice/VolumeNoice1.dds", "VolumeTexture1");
	TextureManager::LoadVolumeTexture("Noice/VolumeNoice/VolumeNoice2.dds", "VolumeTexture2");
	TextureManager::LoadVolumeTexture("Noice/VolumeNoice/VolumeNoice3.dds", "VolumeTexture3");
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
	TextureManager::DestroyRenderTexture("BloomGaussianBlurHalf");
	TextureManager::DestroyRenderTexture("Bloom");
	TextureManager::DestroyRenderTexture("BloomTarget");

	// ラジアルブラー
	TextureManager::DestroyRenderTexture("RadialBlur");

	// ボリュームテクスチャ
	TextureManager::DestroyVolumeTexture("VolumeTexture0");
	TextureManager::DestroyVolumeTexture("VolumeTexture1");
	TextureManager::DestroyVolumeTexture("VolumeTexture2");
	TextureManager::DestroyVolumeTexture("VolumeTexture3");
}

// モデル
void LoadManager::GameSceneModelLoad()
{
	// フィールド
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
	ModelManager::LoadObjModel("Tower", "Tower");
	ModelManager::LoadObjModel("Bridge", "Bridge");

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

	// ポンデリング
	ModelManager::LoadObjModel("PonDeRing", "PonDeRing");
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

	ModelManager::DestroyModel("PonDeRing");
}

///////////////////////////////////////////////////////////////
// --- ゲーム起動時 ----------------------------------------///
///////////////////////////////////////////////////////////////
bool LoadManager::ModelLoad()
{
	//ModelManager::LoadObjModel("Cube", "Cube");
	//ModelManager::LoadObjModel("Sphere", "Sphere");
	//ModelManager::LoadObjModel("Plane", "Plane");
	//ModelManager::LoadObjModel("Capsule", "Capsule");

	// 非同期終わったよ～
	return true;
}
bool LoadManager::TextureLoad()
{
	// タイトルロゴ
	TextureManager::LoadTexture("Title/TitleLogo.dds", "TitleLogo");

	// ディゾルブ
	TextureManager::LoadTexture("DissolveTexture.dds", "DissolveTexture");

	// ノイズ
	TextureManager::LoadTexture("Noice/BlurNoice.dds", "BlurNoice");
	TextureManager::LoadTexture("Noice/ScreenNoice.dds", "ScreenNoice");

	TextureManager::LoadTexture("Text/ExitStr.dds", "ExitStr");

	// シャドウマップ
	RenderTextureSetting setting = RenderTextureSetting(Vec2(1920, 1080) * 2, Vec2(1920, 1080) * 2, true);
	TextureManager::CreateRenderTexture(setting, "ShadowMap");

	TitleSceneLoad();

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
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), "Scene");


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
