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
	TextureManager::CreateRenderTexture(Vec2(1920, 1080), 1, "Skydome");
}

void GameScene::UnLoad()
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

	TextureManager::UnLoadRenderTexture("Skydome");
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
}
void GameScene::Update()
{
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

	// 遷移の処理
	auto currentTransition = TransitionManager::GetInstance()->GetCurrentTransition();
	if (currentTransition == nullptr)
	{
		// タイトルにもどる時
		if (isBackToTitle == true)
		{
			TransitionManager::GetInstance()->Start(TransitionType::Scene);
			SceneChanger::GetInstance()->SetisEaseGameBGM(true);
		}
		// リザルトの時
		else if (ResultUI::GetisEnd() == true)
		{
			TransitionManager::GetInstance()->Start(TransitionType::Scene);
			SceneChanger::GetInstance()->SetisEaseGameBGM(true);
		}
	}
	else
	{
		// シーンの遷移
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
				// プレイヤーが死んだ場合
				if (mPlayer->GetisDissolve() == true)
				{
					// シーンだらゲームシーンを初期化
					SceneManager::ChangeScene<GameScene>();
					mIsChangeScene = true;
				}
				else if (mBoss->GetisDissolve() == true)
				{
					// クリアしたらタイトルに戻す
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

		// リスポーンの遷移
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