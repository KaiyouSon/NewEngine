#pragma once
#include "IScene.h"
#include "Util.h"
#include "LightManager.h"
#include <future>
#include <thread>

// 前方宣言
template<typename T> class Singleton;

// シーンを管理するクラス
class SceneManager : public Singleton<SceneManager>
{
private:
	enum ChangeStep
	{
		None,
		CreateInstance,	// インスタンス生成中
		Loading,		// ロード中
		Changed			// シーン切り替え終わった
	};

private:
	friend Singleton<SceneManager>;
	static std::unique_ptr<IScene> sCurrentScene;
	static std::unique_ptr<IScene> sNextScene;
	ChangeStep mChangeStep;
	bool mIsReturn;

public:
	SceneManager();
	~SceneManager();

public:
	void Init();
	void Update();
	void DrawPass();
	void Draw();
	void DrawDebugGui();

public:
	template<typename T>
	static void ChangeScene()
	{
		switch (GetInstance()->mChangeStep)
		{
		case None:
		{
			GetInstance()->mChangeStep = CreateInstance;
		}
		break;

		case CreateInstance:
		{
			// 次のシーンのインスタンスを作成
			sNextScene = std::make_unique<T>();
			GetInstance()->mChangeStep = Loading;
		}
		break;

		case Loading:
		{
			GetInstance()->mIsReturn = true;

			std::future<bool> ftr = std::async(std::launch::async,
				[]()
				{
					// 現在のシーンのアンロード
					sCurrentScene->UnLoad();

					// 次のシーンのロード
					sNextScene->Load();

					// ライトの初期化
					LightManager::GetInstance()->Init();

					// 次のシーンのインスタンス生成
					sNextScene->CreateInstance();

					// 次のシーンの初期化
					sNextScene->Init();

					return true;
				});

			// スレッド実行終わったらmoveする
			if (ftr.get() == true)
			{
				sCurrentScene = std::move(sNextScene);
				GetInstance()->mChangeStep = Changed;
			}
		}
		break;
		}
	}

	static bool GetisLoading();
	static bool GetisChanged();

	static void SetChangeStepToCreateInstance();
};
