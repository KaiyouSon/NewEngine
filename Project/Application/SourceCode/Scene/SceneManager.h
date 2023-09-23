#pragma once
#include "IScene.h"
#include "Util.h"
#include <future>
#include <thread>

template<typename T> class Singleton;

class SceneManager : public Singleton<SceneManager>
{
private:
	enum ChangeStep
	{
		CreateInstance,
		Loading,
		Changed
	};

private:
	friend Singleton<SceneManager>;
	static std::unique_ptr<IScene> sCurrentScene;
	static std::unique_ptr<IScene> sNextScene;
	static bool sIsChanged;
	ChangeStep mChangeStep;

	Timer mTestTimer;

public:
	SceneManager();
	~SceneManager();

public:
	void Init();
	void Update();
	void RenderTextureSetting();
	void Draw();
	void DrawDebugGui();

public:
	static bool InitNextScene();

	template<typename T>
	static void ChangeScene()
	{
		sIsChanged = false;

		switch (GetInstance()->mChangeStep)
		{
		case CreateInstance:
		{
			// 次のシーンのインスタンスを作成
			sNextScene = std::make_unique<T>();
			GetInstance()->mChangeStep = Loading;
		}
		break;

		case Loading:
		{
			std::future<void> ftr = std::async(std::launch::async,
				[]()
				{
					if (GetInstance()->mTestTimer.GetTimer() == 0)
					{
						// 現在のシーンのアセットをアンロードする
						sCurrentScene->UnLoad();

						// シーン内で使うアセットのロード
						sNextScene->Load();

						// シーン内で使うインスタンス生成
						sNextScene->CreateInstance();

						// シーン初期化
						sNextScene->Init();
					}

					GetInstance()->mTestTimer.Update();
					if (GetInstance()->mTestTimer == true)
					{
						sCurrentScene = std::move(sNextScene);

						GetInstance()->mChangeStep = Changed;
					}
				});
		}
		break;

		case Changed:
		{
			//sCurrentScene = std::move(sNextScene);

			// 次のための準備
			GetInstance()->mChangeStep = CreateInstance;

			// 終わったのを知らせるため
			sIsChanged = true;
		}
		break;

		}
	}

	static bool GetisLoading();
	static bool GetisChanged();
};

