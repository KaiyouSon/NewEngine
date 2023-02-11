#pragma once
#include "Singleton.h"

template<typename T> class Singleton;

class LoadManager :public Singleton<LoadManager>
{
private:
	friend Singleton<LoadManager>;
	LoadManager() {}

private:
	void ModelLoad();
	void TextureLoad();
	void SoundLoad();

public:
	void Load();
};

