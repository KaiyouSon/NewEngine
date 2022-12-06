#pragma once
#include "Util.h"

template<typename T> class Singleton;

class GuiManager :
	public Singleton<GuiManager>
{
private:
	friend Singleton<GuiManager>;
	static const int NumFramesInFlight;

public:
	void Init();
	void Update();
	void Draw();
	void Destroy();
};