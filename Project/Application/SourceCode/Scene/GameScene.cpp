#include "GameScene.h"
#include "TitleScene.h"
#include "SceneChanger.h"
#include "CollisionManager.h"
#include "EffectManager.h"
#include "LogoutMenu.h"
#include "ShadowMap.h"
#include "TransitionManager.h"
#include "CameraManager.h"
#include "MovieCamera.h"
#include "LoadManager.h"

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
	LoadManager::GetInstance()->GameSceneLoad();
}

void GameScene::UnLoad()
{
	LoadManager::GetInstance()->GameSceneUnLoad();
}

void GameScene::CreateInstance()
{
	mPlayer = std::make_unique<Player>();
	mBoss = std::make_unique<Boss>();
	mUiManager = std::make_unique<UIManager>();
	mMenuManager = std::make_unique<MenuManager>();
	mField = std::make_unique<Field>();
	mPostEffectManager = std::make_unique<PostEffectManager>();
	mMovieEvent = std::make_unique<MovieEvent>();
}

void GameScene::Init()
{
	mCurrentScene = TextureManager::GetRenderTexture("CurrentScene");

	Camera::current.pos = Vec3(-65, 75, -85);
	Camera::current.rot = Radian(Vec3(25, 40, 0));

	mPlayer->Init();
	mPlayer->SetMovieEvent(mMovieEvent.get());

	mBoss->Init();
	mBoss->SetPlayer(mPlayer.get());

	mUiManager->SetPlayer(mPlayer.get());
	mUiManager->SetBoss(mBoss.get());
	mUiManager->SetMovieEvent(mMovieEvent.get());
	mUiManager->Init();

	CameraManager::GetInstance()->SetPlayer(mPlayer.get());
	CameraManager::GetInstance()->SetBoss(mBoss.get());
	CameraManager::GetInstance()->Init();
	MovieCamera::SetMovieEvent(mMovieEvent.get());

	mMenuManager->Init();
	mMenuManager->SetMovieEvent(mMovieEvent.get());

	mField->Init();

	mPostEffectManager->SetPlayer(mPlayer.get());
	mPostEffectManager->SetBoss(mBoss.get());
	mPostEffectManager->SetField(mField.get());
	mPostEffectManager->Init();

	ShadowMap::GetInstance()->Init();
	ShadowMap::GetInstance()->Register(128);

	CollisionManager::GetInstance()->SetPlayer(mPlayer.get());
	CollisionManager::GetInstance()->SetBoss(mBoss.get());
	CollisionManager::GetInstance()->SetField(mField.get());
	CollisionManager::GetInstance()->SetUIManager(mUiManager.get());
	CollisionManager::GetInstance()->SetMovieEvent(mMovieEvent.get());

	EffectManager::GetInstance()->Init();
	EffectManager::GetInstance()->SetPlayer(mPlayer.get());

	LightManager::GetInstance()->directionalLight.isActive = true;
	LightManager::GetInstance()->directionalLight.pos = Vec3(-400, 400, -100);

	mMovieEvent->Init();
	mMovieEvent->SetPlayer(mPlayer.get());
	mMovieEvent->SetPlayerCoffin(mField->GetFieldData()->coffins[10].get());
	mMovieEvent->Start();

	isInit = false;

	mIsChangeScene = false;

	//mBoundingBox.SetTexture(TextureManager::GetVolumeTexture("Volume"));
	mBoundingBox.SetTexture(TextureManager::GetVolumeTexture("VolumeTexture"));
	mBoundingBox.scale = 10.f;
}
void GameScene::Update()
{
	if (Key::GetKeyDown(DIK_RETURN))
	{
		mPlayer->Init();
		mMovieEvent->End();
		CameraManager::GetInstance()->ChangeCamera(CameraManager::CameraType::Default);
	}

	if (SoundManager::GetIsPlaying("BattleBGM") == true)
	{
		SoundManager::SetVolume("BattleBGM", mBgmVolume);
		mBgmVolume += 0.005f;
		mBgmVolume = Min<float>(mBgmVolume, 1.f);
	}

	mMovieEvent->Update();

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

	mField->Update();
	mMenuManager->Update();
	mPostEffectManager->Update();
	mBoundingBox.Update();

	ShadowMap::GetInstance()->Update();

	EffectManager::GetInstance()->Update();

	ColliderDrawer::GetInstance()->Update();

	if (mMenuManager->GetisActive() == false)
	{
		CameraManager::GetInstance()->Update();
	}

	bool isBackToTitle =
		LogoutMenu::GetisEnd() == true &&
		LogoutMenu::GetSelect() == LogoutMenu::Select::BackToTitle;

	// 驕ｷ遘ｻ縺ｮ蜃ｦ逅・
	auto currentTransition = TransitionManager::GetInstance()->GetCurrentTransition();
	if (currentTransition == nullptr)
	{
		// 繧ｿ繧､繝医Ν縺ｫ繧ゅ←繧区凾
		if (isBackToTitle == true)
		{
			TransitionManager::GetInstance()->Start(TransitionType::Scene);
			SceneChanger::GetInstance()->SetisEaseGameBGM(true);
		}
		// 繝ｪ繧ｶ繝ｫ繝医・譎・
		else if (ResultUI::GetisEnd() == true)
		{
			TransitionManager::GetInstance()->Start(TransitionType::Scene);
			SceneChanger::GetInstance()->SetisEaseGameBGM(true);
		}
	}
	else
	{
		// 繧ｷ繝ｼ繝ｳ縺ｮ驕ｷ遘ｻ
		if (currentTransition->GetType() == TransitionType::Scene &&
			currentTransition->GetStep() == TransitionStep::Progress)
		{
			if (isBackToTitle == true)
			{
				SceneManager::ChangeScene<TitleScene>();
				mIsChangeScene = true;
			}
			else
			{
				// 繝励Ξ繧､繝､繝ｼ縺梧ｭｻ繧薙□蝣ｴ蜷・
				if (mPlayer->GetisDissolve() == true)
				{
					// 繧ｷ繝ｼ繝ｳ縺繧峨ご繝ｼ繝繧ｷ繝ｼ繝ｳ繧貞・譛溷喧
					SceneManager::ChangeScene<GameScene>();
					mIsChangeScene = true;
				}
				else if (mBoss->GetisDissolve() == true)
				{
					// 繧ｯ繝ｪ繧｢縺励◆繧峨ち繧､繝医Ν縺ｫ謌ｻ縺・
					SceneManager::ChangeScene<TitleScene>();
					mIsChangeScene = true;
				}
			}

			if (mIsChangeScene == true)
			{
				if (SceneManager::GetisChanged() == true)
				{
					TransitionManager::GetInstance()->End();
				}
			}
			else
			{
				TransitionManager::GetInstance()->End();
				mPlayer->Init();
				mMovieEvent->End();

				CameraManager::GetInstance()->ChangeCamera(CameraManager::CameraType::Default);
			}
		}

		// 繝ｪ繧ｹ繝昴・繝ｳ縺ｮ驕ｷ遘ｻ
		if (currentTransition->GetType() == TransitionType::Respawn &&
			currentTransition->GetStep() == TransitionStep::Progress)
		{
			Init();
			TransitionManager::GetInstance()->End();
		}
	}
}

void GameScene::RenderTextureSetting()
{
	ShadowMap::GetInstance()->RenderTextureSetting();
	mField->RenderTextureSetting();

	mPostEffectManager->RenderTextureSetting();
}

void GameScene::Draw()
{
	ShadowMap::GetInstance()->DrawPostEffect();

	mPostEffectManager->DrawEffectBloom();


	mUiManager->DrawFrontSprite();
	mMenuManager->DrawFrontSprite();

	mBoundingBox.Draw();
}

void GameScene::DrawDebugGui()
{
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
	}

	//float height = ShadowMap::sLightCamera.rect.top;
	//GuiManager::DrawSlider1("Camera Rect Height", ShadowMap::sLightCamera.rect.top, 1.f);
	//ShadowMap::sLightCamera.rect.top = +height;
	//ShadowMap::sLightCamera.rect.bottom = -height;

	//GuiManager::EndWindow();

	//mField->DrawDebugGui();

	//mPlayer->DrawDebugGui();
}
