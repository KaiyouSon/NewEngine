#pragma once
#include "Singleton.h"

template<typename T> class Singleton;

class LoadManager :public Singleton<LoadManager>
{
private:
	bool mIsLoaded;

private:
	bool ModelLoad();
	bool TextureLoad();
	bool SoundLoad();
	bool MotionLoad();

public:
	void GameSceneLoad();
	void GameSceneUnLoad();

	void Load();

public:
	inline bool GetisLoaded() { return mIsLoaded; }

private:
	friend Singleton<LoadManager>;
	LoadManager();
};

