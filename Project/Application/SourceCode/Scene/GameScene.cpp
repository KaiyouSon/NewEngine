#include "GameScene.h"
#include "SceneChanger.h"
#include "CollisionManager.h"
#include "EffectManager.h"
#include "LogoutMenu.h"
#include "TitleScene.h"

GameScene::GameScene() :
	player_(std::make_unique<Player>()),
	boss_(std::make_unique<Boss>()),
	uiManager_(std::make_unique<UIManager>()),
	cameraManager_(std::make_unique<CameraManager>()),
	menuManager_(std::make_unique<MenuManager>()),
	field_(std::make_unique<Field>())
{
	currentScene_ = std::make_unique<PostEffect>();
	tex_ = TextureManager::GetRenderTexture("CurrentScene");
	currentScene_->AddRenderTexture(tex_);
	currentScene_->anchorPoint = 0;
	currentScene_->scale = 0.5f;
	currentScene_->pos = GetWindowHalfSize() / 2;
}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	player_->Init();

	boss_->Init();
	boss_->SetPlayer(player_.get());

	uiManager_->SetPlayer(player_.get());
	uiManager_->SetBoss(boss_.get());
	uiManager_->Init();

	cameraManager_->SetPlayer(player_.get());
	cameraManager_->SetBoss(boss_.get());
	cameraManager_->Init();

	menuManager_->Init();

	field_->Init();

	CollisionManager::GetInstance()->SetPlayer(player_.get());
	CollisionManager::GetInstance()->SetBoss(boss_.get());
	CollisionManager::GetInstance()->SetField(field_.get());
	CollisionManager::GetInstance()->SetUIManager(uiManager_.get());

	EffectManager::GetInstance()->Init();

	LightManager::GetInstance()->directionalLight.isActive = true;
	LightManager::GetInstance()->directionalLight.pos = Vec3(-1, 1, -1);

	SceneChanger::GetInstance()->SetisEaseTitleBGM(false);

	SoundManager::Play("BattleBGM", true);
	SoundManager::SetVolume("BattleBGM", 0);
	bgmVolume_ = 0;
}
void GameScene::Update()
{
	if (SceneChanger::GetInstance()->GetisSceneChanging() == false)
	{
		SoundManager::SetVolume("BattleBGM", bgmVolume_);
		bgmVolume_ += 0.01f;
		bgmVolume_ = Min<float>(bgmVolume_, 1.f);
	}
	//// 仮のここに書いた
	//Camera prevCamera = Camera::current;

	//Camera::current.pos = LightManager::GetInstance()->directionalLight.pos;
	//Camera::current.rot = Vec3(Radian(90), 0, 0);
	//Camera::current.Update();
	//player_->PostUpdate();
	//boss_->Update();

	//// SRVヒープの設定コマンド
	//auto temp = TextureManager::GetSrvDescHeap();
	//RenderBase::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, &temp);
	//tex_->PrevDrawScene();

	//RenderBase::GetInstance()->SetObject3DDrawCommand();
	//player_->DrawModel();
	//boss_->DrawModel();
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
		player_->PrevUpdate();
		boss_->Update();
		uiManager_->Update();
		CollisionManager::GetInstance()->Update();
		player_->PostUpdate();
	}

	menuManager_->Update();
	field_->Update();
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
			if (player_->GetisDissolve() == true)
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
}

void GameScene::DrawRenderTexture()
{
	//currentScene_->Draw();
}
void GameScene::DrawBackSprite()
{
}
void GameScene::DrawModel()
{
	player_->DrawModel();
	boss_->DrawModel();
	field_->DrawModel();

	EffectManager::GetInstance()->DrawModel();
}
void GameScene::DrawFrontSprite()
{
	uiManager_->DrawFrontSprite();

	menuManager_->DrawFrontSprite();
}
void GameScene::DrawDebugGui()
{
	GuiManager::BeginWindow("Lighting");
	GuiManager::DrawCheckBox("isActive", &LightManager::GetInstance()->directionalLight.isActive);
	GuiManager::DrawSlider3("pos", LightManager::GetInstance()->directionalLight.pos, 0.01f);
	GuiManager::DrawColorEdit("color", LightManager::GetInstance()->directionalLight.color);
	GuiManager::EndWindow();

	field_->DrawDebugGui();

	player_->DrawDebugGui();
}