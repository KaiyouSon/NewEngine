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
#include "DebugManager.h"

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
	mDirectionalLight = std::make_unique<DirectionalLight>();
	mPointLight = std::make_unique<PointLight>();
	mSpotLight = std::make_unique<SpotLight>();

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
	GraphicsManager::GetDistanceFog()->data.isActive = true;
	GraphicsManager::GetDistanceFog()->data.isActiveHeight = true;
	GraphicsManager::GetDistanceFog()->data.color = Color(30, 30, 15);
	GraphicsManager::GetDistanceFog()->data.nearFarDistance = Vec2(2000, 3500);
	GraphicsManager::GetDistanceFog()->data.nearFarHeight = Vec2(-100, -1000);
	GraphicsManager::GetDistanceFog()->data.distanceRate = Vec3(1.f, 0.f, 1.f);

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

	mField->SetFieldData(FieldDataManager::GetFieldData("SkyIsland"));
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

	mDirectionalLight->pos = Vec3(-100, 440, 440);

	mMovieEvent->Init();
	mMovieEvent->SetPlayer(mPlayer.get());
	mMovieEvent->SetPlayerCoffin(mField->GetFieldData()->coffins[10].get());
	mMovieEvent->Start();

	isInit = false;

	mIsChangeScene = false;

	mVolumetricFog->SetTexture(TextureManager::GetVolumeTexture("VolumeTexture3"));
	mVolumetricFog->pos.z = 400;
	mVolumetricFog->scale = Vec3(1000, 1000, 1400);
	mVolumetricFog->tiling = Vec3(40, 40, 40);
	mVolumetricFog->moveSpeed = -0.000025f;
	mVolumetricFog->fogParam.stepCount = 100;
	mVolumetricFog->fogParam.stepLength = 2.f;
	mVolumetricFog->fogParam.dencity = 0.0015f;
	mVolumetricFog->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("WorldVolumetricFog"));

	VolumetricFog::fogClamp = Vec2(20, 40);

	mPointLight->isActive = false;

	mSpotLight->pos.y = 10;
	mSpotLight->decay = 2.0f;
	mSpotLight->cosAngle = 60;
}
void GameScene::Update()
{
	int32_t offsetZ = (int32_t)(mPlayer->GetPos().z / 200) * 100;
	Vec3 viewLightPos = mDirectionalLight->pos + Vec3(0, 0, (float)offsetZ);
	ShadowMap::GetInstance()->LightViewUpdate(viewLightPos);

	// デバッグ機能
	ProcessAtDebugBuild([this]
		{
			// ホットリロード
			if (Key::GetKey(DIK_H))
			{
				if (Key::GetKeyDown(DIK_S))
				{
					FieldDataManager::Load("SkyIsland", "SkyIsland");
					mField->SetFieldData(FieldDataManager::GetFieldData("SkyIsland"));
					mField->Init();
				}
			}

			if (Key::GetKeyDown(DIK_L))
			{
				Camera::current.pos = Vec3(0, 10, -20);
				Camera::current.rot = Radian(Vec3(0, 0, 0));
			}

			if (Key::GetKeyDown(DIK_F6))
			{
				mPlayer->Init();
				mMovieEvent->End();
				CameraManager::GetInstance()->ChangeCamera(CameraManager::CameraType::Default);
			}
		});

	auto currentTransition = TransitionManager::GetInstance()->GetCurrentTransition();
	if (currentTransition == nullptr)
	{
		if (SoundManager::GetIsPlaying("BattleBGM") == true)
		{
			SoundManager::SetVolume("BattleBGM", 0);
			mBgmVolume += 0.005f;
			mBgmVolume = Min<float>(mBgmVolume, 1.f);
		}
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

	mPostEffectManager->SetRadialBlurCenterPos(mField->GetFieldData()->suns[0]->GetPos());

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
		if (DebugManager::GetInstance()->GetisActive() == false)
		{
			CameraManager::GetInstance()->Update();
		}
		else
		{
			Camera::DebugCameraUpdate();
		}
	}

	// シーン切り替えの処理
	SceneChangeUpdate();
}
void GameScene::ExecuteCS()
{
	EffectManager::GetInstance()->ExecuteCS();
	mField->ExecuteCS();
}
void GameScene::DrawPass()
{
	ShadowMap::GetInstance()->DrawPass();

	std::function<void()> targetDrawFunc;
	std::function<void()> maskDrawFunc;
	std::function<void()> sceneDrawFunc;

	// --- 天球にベネットをかけるパス ------------------------------//
	// ラムダ式で代入
	targetDrawFunc = [this]()
	{
		DrawSkydome();
	};
	mPostEffectManager->SkydomeVignetteDrawPass(targetDrawFunc);

	// ラムダ式で代入
	targetDrawFunc = [this]()
	{
		mField->DrawSun();
	};
	maskDrawFunc = [this]()
	{
		mField->DrawModel();
		mField->DrawSkyIsLand();

		mPlayer->DrawModel();

		mBoss->DrawModel();

		EffectManager::GetInstance()->DrawEffect(false);
	};
	sceneDrawFunc = [this]()
	{
		DrawCurrentSceneObject();
	};
	mPostEffectManager->RadialBlurDrawPass(targetDrawFunc, maskDrawFunc, sceneDrawFunc);

	// --- エフェクトにブルームをかけるパス ------------------------//
	// ラムダ式で代入
	targetDrawFunc = [this]()
	{
		DrawDepthToEffectBloom();
	};
	sceneDrawFunc = [this]()
	{
		mPostEffectManager->DrawRadialBlur();
		//DrawCurrentSceneObject();
		//mVolumetricFog->Draw();
	};
	mPostEffectManager->EffectBloomDrawPass(targetDrawFunc, sceneDrawFunc);
}
void GameScene::Draw()
{
	mPostEffectManager->DrawEffectBloom();

	mUiManager->DrawFrontSprite();
	mMenuManager->DrawFrontSprite();
	ShadowMap::GetInstance()->DrawPostEffect();
}
void GameScene::DrawDebugGui()
{
	//mBoss->DrawDebugGui();
	//mPostEffectManager->DrawDebugGui();
	ShadowMap::GetInstance()->DrawDebugGui();
	GraphicsManager::DrawDebugGui();

	Gui::BeginWindow("Debug");

	if (Gui::DrawCollapsingHeader("Grass") == true)
	{
		Gui::DrawColorEdit("Ambient", Grass::sMaterialColor.ambient);
		Gui::DrawColorEdit("Diffuse", Grass::sMaterialColor.diffuse);
		Gui::DrawColorEdit("Specular", Grass::sMaterialColor.specular);
		Gui::DrawSlider1("Height Scale", Grass::sPOMData.heightScale, 0.01f);
		Gui::DrawSlider1("Num Layers", Grass::sPOMData.numLayers, 0.1f);
	}

	if (Gui::DrawCollapsingHeader("ShadowMap") == true)
	{
		float bias = Object3D::sShadowBias;
		Gui::DrawSlider1("Shadow Bias", bias, 0.0001f);
		Object3D::sShadowBias = bias;
	}

	if (Gui::DrawCollapsingHeader("Fog") == true)
	{
		Gui::DrawSlider2("Fog Clamp", VolumetricFog::fogClamp, 1.f);
		Gui::DrawInputInt("Step Count", (int&)mVolumetricFog->fogParam.stepCount);
		Gui::DrawSlider1("Step Length", mVolumetricFog->fogParam.stepLength, 0.01f);
		Gui::DrawSlider1("Fog Dencity", mVolumetricFog->fogParam.dencity, 0.01f);
		Gui::DrawColorEdit("Fog Color", mVolumetricFog->fogParam.fogColor);
		Gui::DrawLine();
		Gui::DrawSlider1("Fog Color Rate R", mVolumetricFog->fogParam.fogColorRate.r, 0.01f);
		Gui::DrawSlider1("Fog Color Rate G", mVolumetricFog->fogParam.fogColorRate.g, 0.01f);
		Gui::DrawSlider1("Fog Color Rate B", mVolumetricFog->fogParam.fogColorRate.b, 0.01f);
		Gui::DrawSlider1("Fog Color Rate A", mVolumetricFog->fogParam.fogColorRate.a, 0.01f);
		Gui::DrawLine();
		Gui::DrawSlider3("Fog Pos", mVolumetricFog->pos, 0.01f);
		Gui::DrawSlider3("Fog Scale", mVolumetricFog->scale, 0.01f);
		Vec3 angle = Angle(mVolumetricFog->rot);
		Gui::DrawSlider3("Fog Rot", angle, 1.f);
		mVolumetricFog->rot = Radian(angle);
		Gui::DrawSlider3("Fog Speed", mVolumetricFog->moveSpeed, 0.001f);
		Gui::DrawSlider3("Fog tiling", mVolumetricFog->tiling, 0.001f);

		for (uint32_t i = 0; i < 4; i++)
		{
			std::string tag = "VolumeTexture" + std::to_string(i);
			if (Gui::DrawButton(tag.c_str(), Vec2(64, 32)))
			{
				mVolumetricFog->SetTexture(TextureManager::GetVolumeTexture(tag));
			}

			if (i != 3)
			{
				Gui::DrawTab();
			}
		}
	}

	if (Gui::DrawCollapsingHeader("Directional Light") == true)
	{
		Gui::DrawCheckBox("Directional Light Active", &mDirectionalLight->isActive);
		Gui::DrawSlider3("Directional Light Pos", mDirectionalLight->pos, 0.1f);
		Gui::DrawColorEdit("Directional Light Color", mDirectionalLight->color);
	}

	if (Gui::DrawCollapsingHeader("Point Light") == true)
	{
		Gui::DrawCheckBox("Point Light Active", &mPointLight->isActive);
		Gui::DrawSlider3("Point Light Pos", mPointLight->pos, 0.1f);
		Gui::DrawColorEdit("Point Light Color", mPointLight->color);
		Gui::DrawSlider3("Point Light Color Rate", mPointLight->colorRate);
		Gui::DrawSlider1("Point Light Radius", mPointLight->radius);
		Gui::DrawSlider1("Point Light Decay", mPointLight->decay);
	}

	if (Gui::DrawCollapsingHeader("Spot Light") == true)
	{
		Gui::DrawCheckBox("Spot Light Active", &mSpotLight->isActive);
		Gui::DrawSlider3("Spot Light Vec", mSpotLight->vec, 0.01f);
		Gui::DrawSlider3("Spot Light Pos", mSpotLight->pos, 0.1f);
		Gui::DrawColorEdit("Spot Light Color", mSpotLight->color);
		Gui::DrawSlider3("Spot Light Color Rate", mSpotLight->colorRate);
		Gui::DrawSlider1("Spot Light Radius", mSpotLight->radius);
		Gui::DrawSlider1("Spot Light Decay", mSpotLight->decay);
		Gui::DrawSlider2("Spot Light Factor CosAngle", mSpotLight->cosAngle);
	}

	Gui::EndWindow();
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
	mField->DrawTower();
}

// エフェクトのブルームのポストエフェクトに深度のみ書き込む処理
void GameScene::DrawDepthToEffectBloom()
{
	// プレイヤーの深度のみ書き込む
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mPlayer->DrawModel();
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));

	// フィールド
	mField->DrawModel(true);
	mField->DrawSkyIsLand(true);

	// 太陽
	mField->DrawSun();

	// ボス
	mBoss->DrawModel();

	// エフェクトの描画(ブルーム書けるため深度以外も書き込む)
	EffectManager::GetInstance()->DrawEffect(true);
}

// 現在のシーンのオブジェクトの描画
void GameScene::DrawCurrentSceneObject()
{
	mPostEffectManager->DrawSkydomeVignette();

	mField->DrawModel();
	mField->DrawSkyIsLand();
	mField->DrawSun();

	//mField->DrawFog();
	mPlayer->DrawModel();

	mBoss->DrawModel();

	EffectManager::GetInstance()->DrawEffect(false);


	mVolumetricFog->Draw();
}