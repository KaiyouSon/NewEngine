#pragma once
#include "Singleton.h"

template<typename T> class Singleton;

class LoadManager :public Singleton<LoadManager>
{
private:
	bool isLoaded;

private:
	bool ModelLoad();
	bool TextureLoad();
	bool SoundLoad();
	bool MotionLoad();

public:
	void Load();

public:
	inline bool GetisLoaded() { return isLoaded; }

private:
	friend Singleton<LoadManager>;
	LoadManager();
};

