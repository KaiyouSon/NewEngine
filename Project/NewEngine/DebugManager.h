#pragma once
#include "Singleton.h"

template<typename T> class Singleton;

class DebugManager : public Singleton<DebugManager>
{
private:
	bool mIsActive;

private:
	// LoadedŠÖ˜A
	bool mIsShowLoadedWindow;
	void LoadedWindow(bool* flag);

public:
	void Init();
	void Update();
	void DrawDebugGui();

private:
	friend Singleton<DebugManager>;
};

