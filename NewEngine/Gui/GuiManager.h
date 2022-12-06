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
	void PreDraw();
	void PostDraw();
	void Destroy();

	static bool BeginWindow();
	static void EndWindow();

	static void DrawString(const char* fmt, ...);
	static void DrawCheckBox(const char* label, bool* flag);
	static void DrawSlider(const char* label, Vec3& v);
};