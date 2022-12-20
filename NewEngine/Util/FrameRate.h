#pragma once
#include "Singleton.h"
#include <chrono>

template<typename T> class Singleton;

class FrameRate : public Singleton<FrameRate>
{
private:
	friend Singleton<FrameRate>;

private:
	std::chrono::steady_clock::time_point reference;
	float fps;

public:
	FrameRate();
	void Init(const float& fps);
	void Update();
};

