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
			// 谺｡縺ｮ繧ｷ繝ｼ繝ｳ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧剃ｽ懈・
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
					// 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ縺ｮ繧｢繧ｻ繝・ヨ繧偵い繝ｳ繝ｭ繝ｼ繝峨☆繧・
					sCurrentScene->UnLoad();

					// 繧ｷ繝ｼ繝ｳ蜀・〒菴ｿ縺・い繧ｻ繝・ヨ縺ｮ繝ｭ繝ｼ繝・
					sNextScene->Load();

					// 繧ｷ繝ｼ繝ｳ蜀・〒菴ｿ縺・う繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ逕滓・
					sNextScene->CreateInstance();

					// 繧ｷ繝ｼ繝ｳ蛻晄悄蛹・
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
