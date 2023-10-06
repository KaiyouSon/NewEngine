#pragma once
#include "Singleton.h"
#include "MappingWindow.h"

template<typename T> class Singleton;

class DebugManager : public Singleton<DebugManager>
{
private:
	bool mIsActive;
	bool mIsStop;
	bool mIsNextFrame;

private:
	std::unique_ptr<MappingWindow> mMappingWindow;

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
	DebugManager();
};
