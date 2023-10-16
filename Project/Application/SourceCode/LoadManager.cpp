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
	// 蜃ｦ逅・′邨ゅｏ縺｣縺溘・繧呈蕗縺医ｋ縺溘ａ縲∝ｿ・★true繧定ｿ斐☆
	return true;
}

bool LoadManager::TextureLoad()
{
	TextureManager::LoadTexture("Test.png", "Test");
	TextureManager::LoadTexture("Test1.png", "Test1");

	TextureManager::LoadTexture("Particle/StarParticle.png", "StarParticle");

	// 繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ縺ｫ菴ｿ縺・°繧蔚nLoad縺励↑縺・
	TextureManager::LoadTexture("Title/TitleLogo.png", "TitleLogo");
	TextureManager::LoadTexture("DissolveTexture.png", "DissolveTexture");

	// 繝弱う繧ｺ繝・け繧ｹ繝√Ε
	TextureManager::LoadTexture("Noice/BlurNoice.png", "BlurNoice");
	TextureManager::LoadTexture("Noice/ScreenNoice.png", "ScreenNoice");

	// 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ謠冗判諠・ｱ
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 8, 1, "ShadowMap");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 8, 1, "ShadowMapBlur");

	////TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 2, 1, "ShadowMap");
	////TextureManager::CreateRenderTexture(Vec2(1920, 1080) * 2, 1, "ShadowMapBlur");

	// 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "CurrentScene");

	// 繝悶Ν繝ｼ繝
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "HighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "GaussianBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Bloom");


	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "EffectBloom");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "VolumetricFog");

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

	// 蜃ｦ逅・′邨ゅｏ縺｣縺溘・繧呈蕗縺医ｋ縺溘ａ縲∝ｿ・★true繧定ｿ斐☆
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

	// 蜃ｦ逅・′邨ゅｏ縺｣縺溘・繧呈蕗縺医ｋ縺溘ａ縲∝ｿ・★true繧定ｿ斐☆
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

	return true;
}

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
	TextureManager::LoadTexture("UI/RespawnPoint/RespawnBack.png", "RespawnBack");

	// テキスト
	TextureManager::LoadTexture("Text/ColonStr.png", "ColonStr");
	TextureManager::LoadTexture("Text/Negotiation/ReadMessageStr.png", "ReadMessageStr");
	TextureManager::LoadTexture("Text/Negotiation/RestInLightStr.png", "RestInLightStr");
	TextureManager::LoadTexture("Text/Negotiation/OpenStr.png", "OpenStr");
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

	// 天球
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Vignette");
}

void LoadManager::GameSceneUnLoad()
{
	// UI
	TextureManager::UnLoadTexture("Gauge");
	TextureManager::UnLoadTexture("Buttons");
	TextureManager::UnLoadTexture("NegotiationBack");
	TextureManager::UnLoadTexture("MessageBack");
	TextureManager::UnLoadTexture("MessageSignUI");
	TextureManager::UnLoadTexture("ItemBoxFrame");
	TextureManager::UnLoadTexture("ItemBoxLight");
	TextureManager::UnLoadTexture("BottleUI");
	TextureManager::UnLoadTexture("ClubUI");
	TextureManager::UnLoadTexture("MenuBack");
	TextureManager::UnLoadTexture("MenuTextFrame");
	TextureManager::UnLoadTexture("MenuTextLight");
	TextureManager::UnLoadTexture("ResultBack");
	TextureManager::UnLoadTexture("RespawnBack");

	// テキスト
	TextureManager::UnLoadTexture("ColonStr");
	TextureManager::UnLoadTexture("ReadMessageStr");
	TextureManager::UnLoadTexture("RestInLightStr");
	TextureManager::UnLoadTexture("OpenStr");
	TextureManager::UnLoadTexture("ExitStr");
	TextureManager::UnLoadTexture("TutorialStr1");
	TextureManager::UnLoadTexture("TutorialStr2");
	TextureManager::UnLoadTexture("TutorialStr3");
	TextureManager::UnLoadTexture("TutorialStr4");
	TextureManager::UnLoadTexture("TutorialStr5");
	TextureManager::UnLoadTexture("BackToTitleStr");
	TextureManager::UnLoadTexture("CloseGameStr");
	TextureManager::UnLoadTexture("EnemyFelledStr");
	TextureManager::UnLoadTexture("YouDiedStr");
	TextureManager::UnLoadTexture("DecisionCloseStr");
	TextureManager::UnLoadTexture("NumberSheets");

	// パーティクル
	TextureManager::UnLoadTexture("Particle1");
	TextureManager::UnLoadTexture("Particle2");
	TextureManager::UnLoadTexture("Line");

	// 草
	TextureManager::UnLoadTexture("Weed");
	TextureManager::UnLoadTexture("Branch");

	// 天球
	TextureManager::UnLoadRenderTexture("Vignette");
}

LoadManager::LoadManager() : mIsLoaded(false)
{
}

void LoadManager::Load()
{
	// 繝槭Ν繝√せ繝ｬ繝・ラ縺ｫ蜈･繧句燕縺ｫ菴懊▲縺ｨ縺・
	TextureManager::CreateTexture(Color::white, "White");

	// 髱槫酔譛・
	std::future<bool> textureFtr = std::async(std::launch::async, [this] { return TextureLoad(); });
	std::future<bool> modelFtr = std::async(std::launch::async, [this] { return ModelLoad(); });
	std::future<bool> soundFtr = std::async(std::launch::async, [this] { return SoundLoad(); });
	std::future<bool> motionFtr = std::async(std::launch::async, [this] { return MotionLoad(); });

	// 繝ｭ繝ｼ繝牙ｮ御ｺ・
	if (textureFtr.get() == true &&	// 繝・け繧ｹ繝√Ε繝ｼ
		modelFtr.get() == true &&	// 繝｢繝・Ν
		soundFtr.get() == true &&
		motionFtr.get() == true)	// 繧ｵ繧ｦ繝ｳ繝・
	{
		mIsLoaded = true;
		// 繧ｳ繝槭Φ繝牙ｮ溯｡・
		TextureManager::ExcuteComandList();
	}
}
