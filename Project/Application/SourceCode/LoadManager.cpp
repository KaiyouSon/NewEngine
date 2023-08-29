#include "LoadManager.h"
#include "NewEngine.h"
#include "AssimpLoader.h"
#include "MotionManager.h"
#include "FieldDataManager.h"

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

	ModelManager::LoadObjModel("CoffinTop", "CoffinTop", true);
	ModelManager::LoadObjModel("CoffinBottom", "CoffinBottom", true);
	ModelManager::LoadObjModel("SkyIsland/SkyIsland1", "SkyIsland1", true);
	ModelManager::LoadObjModel("SkyIsland/SkyIsland2", "SkyIsland2", true);
	ModelManager::LoadObjModel("Wall/MainWall", "MainWall");
	ModelManager::LoadObjModel("Wall/Wall1", "Wall1");
	ModelManager::LoadObjModel("Wall/Wall2", "Wall2");

	ModelManager::LoadObjModel("Capsule", "Capsule");
	ModelManager::LoadObjModel("MainGateWall", "MainGateWall");
	// 処理が終わったのを教えるため、必ずtrueを返す
	return true;
}

bool LoadManager::TextureLoad()
{
	TextureManager::CreateDepthTexture(Vec2(1920, 1080));
	TextureManager::LoadTexture("DissolveTexture.png", "DissolveTexture");

	// ノイズテクスチャ
	TextureManager::LoadTexture("Noice/BlurNoice.png", "BlurNoice");
	TextureManager::LoadTexture("Noice/ScreenNoice.png", "ScreenNoice");

	// タイトル
	TextureManager::LoadTexture("Title/GameTitle.png", "GameTitle");
	TextureManager::LoadTexture("Title/TitleLogo.png", "TitleLogo");
	TextureManager::LoadTexture("Title/PressButton.png", "PressButton");
	TextureManager::LoadTexture("Title/PressButtonBack.png", "PressButtonBack");

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
	TextureManager::LoadTexture("UI/RespawnPoint/RespawnBack.png", "RespawnBack");

	// テキスト
	TextureManager::LoadTexture("Text/ColonStr.png", "ColonStr");
	TextureManager::LoadTexture("Text/Negotiation/ReadMessageStr.png", "ReadMessageStr");
	TextureManager::LoadTexture("Text/Negotiation/RestInLightStr.png", "RestInLightStr");
	TextureManager::LoadTexture("Text/ExitStr.png", "ExitStr");
	TextureManager::LoadTexture("Text/Tutorial/TutorialStr1.png", "TutorialStr1");
	TextureManager::LoadTexture("Text/Tutorial/TutorialStr2.png", "TutorialStr2");
	TextureManager::LoadTexture("Text/Tutorial/TutorialStr3.png", "TutorialStr3");
	TextureManager::LoadTexture("Text/Tutorial/TutorialStr4.png", "TutorialStr4");
	TextureManager::LoadTexture("Text/Tutorial/TutorialStr5.png", "TutorialStr5");
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

	// レンダーテクスチャー

	// 現在のシーン描画情報
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 8, 1, "ShadowMap");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 8, 1, "ShadowMapBlur");

	//TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 2, 1, "ShadowMap");
	//TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 2, 1, "ShadowMapBlur");

	TextureManager::CreateRenderTexture(Vec2(1920, 1080) , 1, "Skydome");

	// ブルーム
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "HighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "BGaussainBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Bloom");


	// 処理が終わったのを教えるため、必ずtrueを返す
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

	// 処理が終わったのを教えるため、必ずtrueを返す
	return true;
}

bool LoadManager::MotionLoad()
{
	MotionManager::Load("Player/BackstepMotion", "Backstep");
	MotionManager::Load("Player/RollMotion", "Roll");
	MotionManager::Load("Player/DrinkMotion", "Drink");
	MotionManager::Load("Weapon/Mallet/WeakAttackMotion1", "MalletWeakAttack1");
	MotionManager::Load("Weapon/Mallet/WeakAttackMotion2", "MalletWeakAttack2");
	MotionManager::Load("Weapon/Mallet/WeakAttackMotion3", "MalletWeakAttack3");
	MotionManager::Load("Weapon/Mallet/HeavyAttackMotion", "MalletHeavyAttack");
	MotionManager::Load("Weapon/Mallet/BackAttackMotion", "MalletBackAttack");
	MotionManager::Load("Weapon/Mallet/RollAttackMotion", "MalletRollAttack");

	MotionManager::Load("Boss/GrabAttack", "BossGrabAttack");
	MotionManager::Load("Boss/Attack2", "BossAttack2");
	MotionManager::Load("Boss/Attack3", "BossAttack3");

	return true;
}

LoadManager::LoadManager() : mIsLoaded(false)
{
}

void LoadManager::Load()
{
	// マルチスレッドに入る前に作っとく
	TextureManager::CreateTexture(Color::white, "White");

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
		mIsLoaded = true;
		// コマンド実行
		TextureManager::ExcuteComandList();
	}
}