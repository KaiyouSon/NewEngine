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
		End
	};

private:
	friend Singleton<SceneManager>;
	static std::unique_ptr<IScene> sCurrentScene;
	static std::unique_ptr<IScene> sNextScene;
	static bool sIsChanged;
	ChangeStep mChangeStep;

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
					// 現在のシーンのアセットをアンロードする
					sCurrentScene->UnLoad();

					// シーン内で使うアセットのロード
					sNextScene->Load();

					// シーン内で使うインスタンス生成
					sNextScene->CreateInstance();

					// シーン初期化
					sNextScene->Init();

					GetInstance()->mChangeStep = End;
				});
		}
		break;

		case End:
		{
			sCurrentScene = std::move(sNextScene);

			// 次のための準備
			GetInstance()->mChangeStep = CreateInstance;

			// 終わったのを知らせるため
			sIsChanged = true;
		}
		break;

		}
	}

	static bool GetisChanged();
};

