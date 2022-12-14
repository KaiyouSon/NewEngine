#pragma once
#include "IScene.h"
#include "Util.h"

template<typename T> class Singleton;

class SceneManager : public Singleton<SceneManager>
{
private:
	friend Singleton<SceneManager>;
	static std::unique_ptr<IScene> currentScene;

public:
	SceneManager();
	~SceneManager();

public:
	void Init();
	void Update();
	void Draw();

	template<typename T>
	static void ChangeScene()
	{
		std::unique_ptr<IScene> nextScene = std::make_unique<T>();
		nextScene->Init();
		currentScene = std::move(nextScene);
	}
};

