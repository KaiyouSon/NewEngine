#include "GameScene.h"
#include "SceneChanger.h"
#include "CollisionManager.h"
#include "EffectManager.h"
#include "LogoutMenu.h"
#include "TitleScene.h"

GameScene::GameScene() :
	mPlayer(std::make_unique<Player>()),
	mBoss(std::make_unique<Boss>()),
	mUiManager(std::make_unique<UIManager>()),
	mCameraManager(std::make_unique<CameraManager>()),
	mMenuManager(std::make_unique<MenuManager>()),
	mField(std::make_unique<Field>())
{

	SceneChanger::GetInstance()->SetisEaseTitleBGM(false);
	mBgmVolume = 0;

	mRespawnTransition = nullptr;
	mRespawnTransition = std::make_unique<RespawnTransition>();
	mRespawnTransition->Generate();
}
GameScene::~GameScene()
{
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

	CollisionManager::GetInstance()->SetPlayer(mPlayer.get());
	CollisionManager::GetInstance()->SetBoss(mBoss.get());
	CollisionManager::GetInstance()->SetField(mField.get());
	CollisionManager::GetInstance()->SetUIManager(mUiManager.get());

	EffectManager::GetInstance()->Init();

	LightManager::GetInstance()->directionalLight.isActive = true;
	LightManager::GetInstance()->directionalLight.pos = Vec3(-400, 400, -100);
}
void GameScene::Update()
{
	static bool isInit = false;

	if (Key::GetKeyDown(DIK_SPACE))
	{
		mRespawnTransition->Generate();
	}

	if (Key::GetKeyDown(DIK_A))
	{
		isInit = false;
		mRespawnTransition->SetStep(RespawnTransition::In);
	}
	if (Key::GetKeyDown(DIK_D))
	{
		mRespawnTransition->SetStep(RespawnTransition::Out);
	}

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

	mShadowMap.Update();

	EffectManager::GetInstance()->Update();

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
			// �v���C���[�����񂾏ꍇ
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

	if (mRespawnTransition != nullptr)
	{
		if (mRespawnTransition->GetStep() == RespawnTransition::Progress)
		{
			if (isInit == false)
			{
				Init();
				isInit = true;
			}
		}

		mRespawnTransition->Update();
	}
}

void GameScene::RenderTextureSetting()
{
	mShadowMap.RenderTextureSetting();
}

void GameScene::DrawRenderTexture()
{
	mShadowMap.DrawPostEffect();
}
void GameScene::DrawBackSprite()
{
}
void GameScene::DrawModel()
{
	//mShadowMap.DrawPostEffect();


	mPlayer->DrawModel();
	mBoss->DrawModel();
	mField->DrawModel();

	EffectManager::GetInstance()->DrawModel();

}
void GameScene::DrawFrontSprite()
{
	mUiManager->DrawFrontSprite();

	mMenuManager->DrawFrontSprite();

	if (mRespawnTransition != nullptr)
	{
		mRespawnTransition->DrawFrontSprite();
	}
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