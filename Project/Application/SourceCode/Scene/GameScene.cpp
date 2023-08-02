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
	cameraManager_(std::make_unique<CameraManager>()),
	menuManager_(std::make_unique<MenuManager>()),
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

	cameraManager_->SetPlayer(mPlayer.get());
	cameraManager_->SetBoss(mBoss.get());
	cameraManager_->Init();

	menuManager_->Init();

	mField->Init();

	CollisionManager::GetInstance()->SetPlayer(mPlayer.get());
	CollisionManager::GetInstance()->SetBoss(mBoss.get());
	CollisionManager::GetInstance()->SetField(mField.get());
	CollisionManager::GetInstance()->SetUIManager(mUiManager.get());

	EffectManager::GetInstance()->Init();

	LightManager::GetInstance()->directionalLight.isActive = true;
	LightManager::GetInstance()->directionalLight.pos = Vec3(-1, 10, -1);

	SceneChanger::GetInstance()->SetisEaseTitleBGM(false);

	bgmVolume_ = 0;
}
void GameScene::Update()
{
	if (SceneChanger::GetInstance()->GetisSceneChanging() == false)
	{
		if (SoundManager::GetIsPlaying("BattleBGM") == true)
		{
			SoundManager::SetVolume("BattleBGM", bgmVolume_);
			bgmVolume_ += 0.005f;
			bgmVolume_ = Min<float>(bgmVolume_, 1.f);
		}
	}
	//// 仮のここに書いた
	//Camera prevCamera = Camera::current;

	//Camera::current.pos = LightManager::GetInstance()->directionalLight.pos;
	//Camera::current.rot = Vec3(Radian(90), 0, 0);
	//Camera::current.Update();
	//mPlayer->PostUpdate();
	//mBoss->Update();

	//// SRVヒープの設定コマンド
	//auto temp = TextureManager::GetSrvDescHeap();
	//RenderBase::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, &temp);
	//tex_->PrevDrawScene();

	//RenderBase::GetInstance()->SetObject3DDrawCommand();
	//mPlayer->DrawModel();
	//mBoss->DrawModel();
	//tex_->PostDrawScene();

	//Camera::current = prevCamera;
	//Camera::current.Update();

	//float x = (float)(Key::GetKey(DIK_RIGHT) - Key::GetKey(DIK_LEFT));
	//float y = (float)(Key::GetKey(DIK_DOWN) - Key::GetKey(DIK_UP));
	//currentScene_->pos.x += x;
	//currentScene_->pos.y += y;
	//currentScene_->Update();

	if (menuManager_->GetisActive() == false)
	{
		mPlayer->PrevUpdate();
		mBoss->Update();
		mUiManager->Update();
		CollisionManager::GetInstance()->Update();
		mPlayer->PostUpdate();
	}


	menuManager_->Update();
	mField->Update();
	shadowMap_.Update();
	EffectManager::GetInstance()->Update();

	if (menuManager_->GetisActive() == false)
	{
		cameraManager_->Update();
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
			// プレイヤーが死んだ場合
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
	shadowMap_.RenderTextureSetting();
}

void GameScene::DrawRenderTexture()
{
	shadowMap_.DrawPostEffect();


	//currentScene_->Draw();
}
void GameScene::DrawBackSprite()
{
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

	menuManager_->DrawFrontSprite();
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