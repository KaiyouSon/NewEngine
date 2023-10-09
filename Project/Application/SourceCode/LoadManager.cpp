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

	//TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Skydome");

	// 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "CurrentScene");

	// 繝悶Ν繝ｼ繝
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "HighLumi");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "GaussianBlur");
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Bloom");


	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "EffectBloom");


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
