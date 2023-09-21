#include "GameScene.h"
#include "TitleScene.h"
#include "SceneChanger.h"
#include "CollisionManager.h"
#include "EffectManager.h"
#include "LogoutMenu.h"
#include "ShadowMap.h"
#include "TransitionManager.h"

GameScene::GameScene()
{
	SceneChanger::GetInstance()->SetisEaseTitleBGM(false);
	mBgmVolume = 0;
}
GameScene::~GameScene()
{
}

void GameScene::Load()
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
}

void GameScene::UnLoad()
{
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
}

void GameScene::CreateInstance()
{
	mPlayer = std::make_unique<Player>();
	mBoss = std::make_unique<Boss>();
	mUiManager = std::make_unique<UIManager>();
	mCameraManager = std::make_unique<CameraManager>();
	mMenuManager = std::make_unique<MenuManager>();
	mField = std::make_unique<Field>();
}

void GameScene::Init()
{

	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	mPlayer->Init();

	mBoss->Init();
	mBoss->SetPlayer(mPlayer.get());

	mUiManager->SetPlayer(mPlayer.get());
	mUiManager->SetBoss(mBoss.get());
	mUiManager->Init();

	mCameraManager->SetPlayer(mPlayer.get());
	mCameraManager->SetBoss(mBoss.get());
	mCameraManager->Init();

	mMenuManager->Init();

	mField->Init();

	ShadowMap::GetInstance()->Init();
	ShadowMap::GetInstance()->Register(128);

	CollisionManager::GetInstance()->SetPlayer(mPlayer.get());
	CollisionManager::GetInstance()->SetBoss(mBoss.get());
	CollisionManager::GetInstance()->SetField(mField.get());
	CollisionManager::GetInstance()->SetUIManager(mUiManager.get());

	EffectManager::GetInstance()->Init();

	LightManager::GetInstance()->directionalLight.isActive = true;
	LightManager::GetInstance()->directionalLight.pos = Vec3(-400, 400, -100);

	isInit = false;
}
void GameScene::Update()
{
	if (SceneChanger::GetInstance()->GetisSceneChanging() == false)
	{
		if (SoundManager::GetIsPlaying("BattleBGM") == true)
		{
			SoundManager::SetVolume("BattleBGM", mBgmVolume);
			mBgmVolume += 0.005f;
			mBgmVolume = Min<float>(mBgmVolume, 1.f);
		}
	}

	if (mMenuManager->GetisActive() == false)
	{
		mPlayer->PrevUpdate();
		mBoss->Update();
		mUiManager->Update();
		CollisionManager::GetInstance()->Update();
		mPlayer->PostUpdate();
	}

	auto collider = mPlayer->GetBodyCollider();
	ColliderDrawer::GetInstance()->Bind(&collider);

	mMenuManager->Update();
	mField->Update();

	ShadowMap::GetInstance()->Update();

	EffectManager::GetInstance()->Update();

	ColliderDrawer::GetInstance()->Update();

	if (mMenuManager->GetisActive() == false)
	{
		mCameraManager->Update();
	}

	bool isBackToTitle =
		LogoutMenu::GetisEnd() == true &&
		LogoutMenu::GetSelect() == LogoutMenu::Select::BackToTitle;

	if (isBackToTitle == true)
	{
		if (SceneChanger::GetInstance()->GetisSceneChanging() == false)
		{
			SceneChanger::GetInstance()->StartSceneChange();
			SceneChanger::GetInstance()->SetisEaseGameBGM(true);
		}

		if (SceneChanger::GetInstance()->GetisChange() == true)
		{
			SceneManager::ChangeScene<TitleScene>();
			SceneChanger::GetInstance()->SetisChange(false);
		}
	}
	else if (ResultUI::GetisEnd() == true)
	{
		if (SceneChanger::GetInstance()->GetisSceneChanging() == false)
		{
			SceneChanger::GetInstance()->StartSceneChange();
			SceneChanger::GetInstance()->SetisEaseGameBGM(true);
		}

		if (SceneChanger::GetInstance()->GetisChange() == true)
		{
			// ƒvƒŒƒCƒ„[‚ªŽ€‚ñ‚¾ê‡
			if (mPlayer->GetisDissolve() == true)
			{
				SceneManager::ChangeScene<GameScene>();
			}
			else
			{
				SceneManager::ChangeScene<TitleScene>();
			}

			SceneChanger::GetInstance()->SetisChange(false);
		}
	}

	auto currentTransition = TransitionManager::GetInstance()->GetCurrentTransition();
	if (currentTransition != nullptr)
	{
		if (currentTransition->GetType() == TransitionType::Respawn &&
			currentTransition->GetStep() == TransitionStep::Progress)
		{
			if (isInit == false)
			{
				Init();
				TransitionManager::GetInstance()->End();
				isInit = true;
			}
		}
	}
}

void GameScene::RenderTextureSetting()
{
	ShadowMap::GetInstance()->RenderTextureSetting();
	mField->RenderTextureSetting();
}

void GameScene::Draw()
{
	ShadowMap::GetInstance()->DrawPostEffect();

	mField->DrawModel();
	mPlayer->DrawModel();
	mBoss->DrawModel();

	EffectManager::GetInstance()->DrawModel();

	mUiManager->DrawFrontSprite();
	mMenuManager->DrawFrontSprite();

}

void GameScene::DrawDebugGui()
{
	//GuiManager::BeginWindow("Lighting");
	////GuiManager::DrawCheckBox("isActive", &LightManager::GetInstance()->directionalLight.isActive);
	////GuiManager::DrawColorEdit("color", LightManager::GetInstance()->directionalLight.color);
	//GuiManager::DrawSlider3("Light Pos", LightManager::GetInstance()->directionalLight.pos, 0.01f);

	//Vec3 angle = Angle(ShadowMap::sLightCamera.rot);
	//GuiManager::DrawSlider3("Camera Rot", angle, 1.f);
	//ShadowMap::sLightCamera.rot = Radian(angle);

	//GuiManager::DrawSlider1("Camera NearZ", ShadowMap::sLightCamera.oNearZ, 0.01f);
	//GuiManager::DrawSlider1("Camera FarZ", ShadowMap::sLightCamera.oFarZ, 1.f);

	//float fov = Angle(ShadowMap::sLightCamera.fov);
	//GuiManager::DrawSlider1("Camera Fov", fov, 1.f);
	//ShadowMap::sLightCamera.fov = Radian(fov);

	//float width = ShadowMap::sLightCamera.rect.right;
	//GuiManager::DrawSlider1("Camera Rect Width", width, 1.f);
	//ShadowMap::sLightCamera.rect.left = -width;
	//ShadowMap::sLightCamera.rect.right = +width;

	//float height = ShadowMap::sLightCamera.rect.top;
	//GuiManager::DrawSlider1("Camera Rect Height", ShadowMap::sLightCamera.rect.top, 1.f);
	//ShadowMap::sLightCamera.rect.top = +height;
	//ShadowMap::sLightCamera.rect.bottom = -height;

	//GuiManager::EndWindow();

	//mField->DrawDebugGui();

	//mPlayer->DrawDebugGui();
}