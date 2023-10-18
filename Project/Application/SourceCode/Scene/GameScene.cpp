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

	mBoundingBox.SetTexture(TextureManager::GetVolumeTexture("VolumeTexture"));
	//mBoundingBox.pos.y = 10;
	//mBoundingBox.scale = 10.f;
}
void GameScene::Update()
{
	if (Key::GetKey(DIK_RIGHT))
	{
		mBoundingBox.pos.x += 0.01f;
	}
	if (Key::GetKey(DIK_LEFT))
	{
		mBoundingBox.pos.x -= 0.01f;
	}
	if (Key::GetKey(DIK_UP))
	{
		mBoundingBox.pos.z += 0.01f;
	}
	if (Key::GetKey(DIK_DOWN))
	{
		mBoundingBox.pos.z -= 0.01f;
	}
	if (Key::GetKey(DIK_Q))
	{
		mBoundingBox.scale += 0.01f;
	}
	if (Key::GetKey(DIK_E))
	{
		mBoundingBox.scale -= 0.01f;
	}
	if (Key::GetKeyDown(DIK_L))
	{
		mBoundingBox.pos.y = 10;
		//mBoundingBox.scale.x = 1000;
		//mBoundingBox.scale.y = 25;
		//mBoundingBox.scale.z = 1000;
	}

	//mBoundingBox.scale = 2.f;

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
	mBoundingBox.Update();
	mVolumetricFog->Update();

	ShadowMap::GetInstance()->Update();
	EffectManager::GetInstance()->Update();
	ColliderDrawer::GetInstance()->Update();

	if (mMenuManager->GetisActive() == false)
	{
		//CameraManager::GetInstance()->Update();
	}

	// シーン切り替えの処理
	SceneChangeUpdate();
}
void GameScene::DrawPass()
{
	ShadowMap::GetInstance()->RenderTextureSetting();

	//mVolumetricFog->PrevDrawScene();

	//mVolumetricFog->PostDrawScene();

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

	//mVolumetricFog->Draw();


	mUiManager->DrawFrontSprite();
	mMenuManager->DrawFrontSprite();
}
void GameScene::DrawDebugGui()
{
	Gui::BeginWindow("Debug");

	//Gui::DrawString("Screen Pos = %f,%f", GetWindowHalfSize().x, GetWindowHalfSize().y);
	//Vec3 worldPos = ScreenToWorld(GetWindowHalfSize());
	//Gui::DrawString("World Pos = %f,%f,%f", worldPos.x, worldPos.y, worldPos.z);

	Gui::DrawInputInt("Step Count", (int&)mBoundingBox.fogParam.stepCount);
	Gui::DrawSlider1("Step Length", mBoundingBox.fogParam.stepLength, 0.01f);
	Gui::DrawSlider1("Fog Dencity", mBoundingBox.fogParam.dencity, 0.01f);
	Gui::DrawColorEdit("Fog Color", mBoundingBox.fogParam.fogColor);

	//Gui::DrawLine();
	//Gui::DrawSlider1("Fog Color Rate R", mBoundingBox.fogParam.fogColorRate.r, 0.01f);
	//Gui::DrawSlider1("Fog Color Rate G", mBoundingBox.fogParam.fogColorRate.g, 0.01f);
	//Gui::DrawSlider1("Fog Color Rate B", mBoundingBox.fogParam.fogColorRate.b, 0.01f);
	//Gui::DrawSlider1("Fog Color Rate A", mBoundingBox.fogParam.fogColorRate.a, 0.01f);

	Gui::DrawLine();
	Gui::DrawSlider3("Fog Pos", mBoundingBox.pos, 0.01f);
	Gui::DrawSlider3("Fog Scale", mBoundingBox.scale, 0.01f);
	Gui::DrawSlider3("Fog Speed", mBoundingBox.moveSpeed, 0.001f);

	Gui::EndWindow();


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
	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GrassWriteNone"));
	mField->DrawModel();
	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Grass"));

	// エフェクトの描画(ブルーム書けるため深度以外も書き込む)
	//EffectManager::GetInstance()->DrawEffect(true);
}

// 現在のシーンのオブジェクトの描画
void GameScene::DrawCurrentSceneObject()
{
	mPostEffectManager->DrawSkydomeVignette();
	mField->DrawModel();

	mPlayer->DrawModel();
	mBoss->DrawModel();

	mBoundingBox.Draw();

	//EffectManager::GetInstance()->DrawEffect(false);
}