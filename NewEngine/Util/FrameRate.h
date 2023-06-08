#pragma once
#include "Singleton.h"
#include <chrono>

template<typename T> class Singleton;

class FrameRate : public Singleton<FrameRate>
{
private:
	std::chrono::steady_clock::time_point reference_;
	float frameRate_;

public:
	void Init(const float frameRate);
	void Update();

private:
	friend Singleton<FrameRate>;
	FrameRate();

};

