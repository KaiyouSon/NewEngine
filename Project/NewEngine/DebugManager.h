#pragma once
#include "Singleton.h"

template<typename T> class Singleton;

class DebugManager : public Singleton<DebugManager>
{
private:
	bool mIsActive;
	bool mIsStop;
	bool mIsNextFrame;

private:
	// LoadedŠÖ˜A
	bool mIsShowLoadedWindow;
	void LoadedWindow(bool* flag);

public:
	void Init();
	void Update();
	void DrawDebugGui();

public:
	bool GetisStop();
	bool GetisNextFrame();
	void SetisNextFrame(const bool isNextFrame);

private:
	friend Singleton<DebugManager>;
};

