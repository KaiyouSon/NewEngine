#pragma once
#include "Singleton.h"

// 前方宣言
template<typename T> class Singleton;

// ロードに関連するクラス
class LoadManager :public Singleton<LoadManager>
{
private:
	bool mIsLoaded;

private:
	bool ModelLoad();
	bool TextureLoad();
	bool SoundLoad();
	bool MotionLoad();

private:
	void GameSceneTextureLoad();
	void GameSceneTextureUnLoad();
	void GameSceneModelLoad();
	void GameSceneModelUnLoad();

public:
	// タイトルシーンでのロード・アンロード
	void TitleSceneLoad();
	void TitleSceneUnLoad();

	// ゲームシーンでのロード・アンロード
	void GameSceneLoad();
	void GameSceneUnLoad();

	void Load();

public:
	inline bool GetisLoaded() { return mIsLoaded; }

private:
	friend Singleton<LoadManager>;
	LoadManager();
};

