#pragma once
#include "Singleton.h"
#include <chrono>

template<typename T> class Singleton;

class FrameRate : public Singleton<FrameRate>
{
private:
	std::chrono::steady_clock::time_point mReference;
	float mFrameRate;
	float mCurrentFPS;

public:
	void Init(const float frameRate);
	void Update();

public:
	float GetCurrentFPS();

private:
	friend Singleton<FrameRate>;
	FrameRate();

};
