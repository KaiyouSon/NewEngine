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
	void Draw();
	void DrawDebugGui();

	template<typename T>
	static void ChangeScene()
	{
		// 現在のシーンのアセットをアンロードする
		sCurrentScene->UnLoad();

		// 次のシーンのインスタンスを作成
		std::unique_ptr<IScene> nextScene = std::make_unique<T>();

		// シーン内で使うアセットのロード
		nextScene->Load();

		// シーン内で使うインスタンス生成
		nextScene->CreateInstance();

		// シーン初期化
		nextScene->Init();

		sCurrentScene = std::move(nextScene);
	}
};

