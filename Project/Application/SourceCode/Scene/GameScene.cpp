#include "GameScene.h"
#include "TitleScene.h"
#include "CollisionManager.h"
#include "EffectManager.h"
#include "LogoutMenu.h"
#include "ShadowMap.h"
#include "TransitionManager.h"
#include "CameraManager.h"
#include "MovieCamera.h"
#include "LoadManager.h"
#include "ResultUI.h"
#include "FieldDataManager.h"

void GameScene::Load()
{
	LoadManager::GetInstance()->GameSceneLoad();
	FieldDataManager::Load("SkyIsland", "SkyIsland");
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
	mSkydome = std::make_unique<Skydome>();
	mVolumetricFog = std::make_unique<VolumetricFog>();
}

void GameScene::Init()
{
	mBgmVolume = 0;

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

	EffectManager::GetInstance()->Init();
	EffectManager::GetInstance()->SetPlayer(mPlayer.get());

	mField->Init();
	mSkydome->Init();

	mPostEffectManager->Init();

	ShadowMap::GetInstance()->Init();
	ShadowMap::GetInstance()->Register(128);

	CollisionManager::GetInstance()->SetPlayer(mPlayer.get());
	CollisionManager::GetInstance()->SetBoss(mBoss.get());
	CollisionManager::GetInstance()->SetField(mField.get());
	CollisionManager::GetInstance()->SetUIManager(mUiManager.get());
	CollisionManager::GetInstance()->SetMovieEvent(mMovieEvent.get());

	LightManager::GetInstance()->directionalLight.isActive = true;
	LightManager::GetInstance()->directionalLight.pos = Vec3(-400, 400, -100);

	mMovieEvent->Init();
	mMovieEvent->SetPlayer(mPlayer.get());
	mMovieEvent->SetPlayerCoffin(mField->GetFieldData()->coffins[10].get());
	mMovieEvent->Start();

	isInit = false;

	mIsChangeScene = false;

	mVolumetricFog->SetTexture(TextureManager::GetVolumeTexture("VolumeTexture"));
	mVolumetricFog->scale = Vec3(100.f, 50.f, 100.f);
	mVolumetricFog->fogParam.stepCount = 50;
	mVolumetricFog->fogParam.stepLength = 1.f;
	mVolumetricFog->fogParam.dencity = 0.02f;
	mVolumetricFog->color = Color(233, 216, 187, 255);
	VolumetricFog::fogClamp = Vec2(30.f, 50.f);
}
void GameScene::Update()
{
	Vec3 offset = Vec3(0, 15, 0);
	mVolumetricFog->pos = Camera::current.pos + offset;
	mVolumetricFog->pos.y = Max(mVolumetricFog->pos.y, 25.f);
	mVolumetricFog->moveSpeed = Vec3(0.001f, -0.001f, -0.001f);

	if (Key::GetKeyDown(DIK_L))
	{
		Camera::current.pos = Vec3(0, 10, -20);
		Camera::current.rot = Radian(Vec3(0, 0, 0));

		//mVolumetricFog->pos.y = 10;
		//mVolumetricFog->scale.x = 1000;
		//mVolumetricFog->scale.y = 25;
		//mVolumetricFog->scale.z = 1000;
	}

	if (Key::GetKeyDown(DIK_F10))
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

	// ムービーイベント
	mMovieEvent->Update();

	// ポーズ画面開いてない時のも実行
	if (mMenuManager->GetisActive() == false)
	{
		mPlayer->PrevUpdate();
		mBoss->Update();
		mUiManager->Update();
		CollisionManager::GetInstance()->Update();
		mPlayer->PostUpdate();
	}

	mField->Update();
	mSkydome->Update();
	mMenuManager->Update();
	mPostEffectManager->Update();
	mVolumetricFog->Update();

	ShadowMap::GetInstance()->Update();
	EffectManager::GetInstance()->Update();
	ColliderDrawer::GetInstance()->Update();

	if (mMenuManager->GetisActive() == false)
	{
		CameraManager::GetInstance()->Update();
	}

	// シーン切り替えの処理
	SceneChangeUpdate();
}
void GameScene::DrawPass()
{
	ShadowMap::GetInstance()->RenderTextureSetting();

	std::function<void()> targetDrawFunc;
	std::function<void()> sceneDrawFunc;

	// --- 天球にベネットをかけるパス ------------------------------//
	// ラムダ式で代入
	targetDrawFunc = [this]()
	{
		DrawSkydome();
	};
	mPostEffectManager->SkydomeVignetteDrawPass(targetDrawFunc);

	// --- エフェクトにブルームをかけるパス ------------------------//
	// ラムダ式で代入
	targetDrawFunc = [this]()
	{
		DrawDepthToEffectBloom();
	};
	sceneDrawFunc = [this]()
	{
		DrawCurrentSceneObject();
	};
	mPostEffectManager->EffectBloomDrawPass(targetDrawFunc, sceneDrawFunc);
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
	//Gui::BeginWindow("Debug");

	//if (Gui::DrawCollapsingHeader("Fog 0") == true)
	//{
	//	Gui::DrawSlider2("Fog Clamp", VolumetricFog::fogClamp, 1.f);
	//	Gui::DrawInputInt("Step Count", (int&)mVolumetricFog->fogParam.stepCount);
	//	Gui::DrawSlider1("Step Length", mVolumetricFog->fogParam.stepLength, 0.01f);
	//	Gui::DrawSlider1("Fog Dencity", mVolumetricFog->fogParam.dencity, 0.01f);
	//	Gui::DrawColorEdit("Fog Color", mVolumetricFog->fogParam.fogColor);

	//	Gui::DrawLine();
	//	Gui::DrawSlider1("Fog Color Rate R", mVolumetricFog->fogParam.fogColorRate.r, 0.01f);
	//	Gui::DrawSlider1("Fog Color Rate G", mVolumetricFog->fogParam.fogColorRate.g, 0.01f);
	//	Gui::DrawSlider1("Fog Color Rate B", mVolumetricFog->fogParam.fogColorRate.b, 0.01f);
	//	Gui::DrawSlider1("Fog Color Rate A", mVolumetricFog->fogParam.fogColorRate.a, 0.01f);

	//	Gui::DrawLine();
	//	Gui::DrawSlider3("Fog Pos", mVolumetricFog->pos, 0.01f);
	//	Gui::DrawSlider3("Fog Scale", mVolumetricFog->scale, 0.01f);
	//	Gui::DrawSlider3("Fog Speed", mVolumetricFog->moveSpeed, 0.001f);
	//}
	//Gui::EndWindow();
}

// シーン切り替えの処理
void GameScene::SceneChangeUpdate()
{
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
		}
		// リザルトにもどる時
		else if (ResultUI::GetisEnd() == true)
		{
			TransitionManager::GetInstance()->Start(TransitionType::Scene);
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
					// ゲームシーン
					SceneManager::ChangeScene<GameScene>();
					mIsChangeScene = true;
				}
				else if (mBoss->GetisDissolve() == true)
				{
					// タイトルシーン
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

		// リスポーンの遷移処理
		if (currentTransition->GetType() == TransitionType::Respawn &&
			currentTransition->GetStep() == TransitionStep::Progress)
		{
			Init();
			TransitionManager::GetInstance()->End();
		}
	}
}

// 天球の描画
void GameScene::DrawSkydome()
{
	mSkydome->Draw();
}

// エフェクトのブルームのポストエフェクトに深度のみ書き込む処理
void GameScene::DrawDepthToEffectBloom()
{
	// プレイヤーの深度のみ書き込む
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mPlayer->DrawModel();
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));

	// ボスの深度のみ書き込む
	mBoss->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mBoss->DrawModel();
	mBoss->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));

	// フィールドオブジェクトの深度のみ書き込む
	//mVolumetricFog->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("VolumetricFogWriteNone"));
	//mVolumetricFog->Draw();
	//mVolumetricFog->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("VolumetricFog"));

	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GrassWriteNone"));
	mField->DrawModel();
	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Grass"));

	// エフェクトの描画(ブルーム書けるため深度以外も書き込む)
	EffectManager::GetInstance()->DrawEffect(true);
}

// 現在のシーンのオブジェクトの描画
void GameScene::DrawCurrentSceneObject()
{
	mPostEffectManager->DrawSkydomeVignette();

	mField->DrawModel();

	mPlayer->DrawModel();
	mBoss->DrawModel();

	EffectManager::GetInstance()->DrawEffect(false);

	mVolumetricFog->Draw();
}