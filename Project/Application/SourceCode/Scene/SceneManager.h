#pragma once
#include "IScene.h"
#include "Util.h"

template<typename T> class Singleton;

class SceneManager : public Singleton<SceneManager>
{
private:
	friend Singleton<SceneManager>;
	static std::unique_ptr<IScene> sCurrentScene;

public:
	SceneManager();
	~SceneManager();

public:
	void Init();
	void Update();
	void RenderTextureSetting();
	void DrawBackSprite();
	void DrawModel();
	void DrawFrontSprite();
	void DrawRenderTexture();

	template<typename T>
	static void ChangeScene()
	{
		// 現在のシーンのアセットをアンロードする
		sCurrentScene->UnLoad();

		// 次のシーンのインスタンスを作成
		std::unique_ptr<IScene> nextScene = std::make_unique<T>();
		nextScene->Load();	// ロード
		nextScene->Init();	// 初期化

		sCurrentScene = std::move(nextScene);
	}
};

