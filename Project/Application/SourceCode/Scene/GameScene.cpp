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
	LightManager::GetInstance()->directionalLight.pos = Vec3(-1, 10, -1);

	SceneChanger::GetInstance()->SetisEaseTitleBGM(false);

	mBgmVolume = 0;
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
	//mShadowMap.DrawPostEffect();
}
void GameScene::DrawModel()
{
	mPlayer->DrawModel();
	mBoss->DrawModel();
	mField->DrawModel();

	EffectManager::GetInstance()->DrawModel();
}
void GameScene::DrawFrontSprite()
{
	mUiManager->DrawFrontSprite();

	mMenuManager->DrawFrontSprite();
}
void GameScene::DrawDebugGui()
{
	GuiManager::BeginWindow("Lighting");
	GuiManager::DrawCheckBox("isActive", &LightManager::GetInstance()->directionalLight.isActive);
	GuiManager::DrawSlider3("pos", LightManager::GetInstance()->directionalLight.pos, 0.01f);
	GuiManager::DrawColorEdit("color", LightManager::GetInstance()->directionalLight.color);
	GuiManager::EndWindow();

	//mField->DrawDebugGui();

	//mPlayer->DrawDebugGui();
}