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
	bool mIsReturn;

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
			GetInstance()->mIsReturn = true;

			std::future<bool> ftr = std::async(std::launch::async,
				[]()
				{
					// 現在のシーンのアセットをアンロードする
					sCurrentScene->UnLoad();

					// シーン内で使うアセットのロード
					sNextScene->Load();

					// シーン内で使うインスタンス生成
					sNextScene->CreateInstance();

					// シーン初期化
					sNextScene->Init();

					return true;
				});

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

