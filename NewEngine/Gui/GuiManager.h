#pragma once
#include "imgui.h"
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

	static bool BeginWindow(const char* name);
	static void EndWindow();
	static void DrawDebugWindow(bool& flag);

	static void DrawTab();

	static void DrawString(const char* fmt, ...);
	static void DrawCheckBox(const char* label, bool* flag);

	static bool DrawRadioButton(const char* label, int& current, const int& index, const bool& isTab = true);

	static void DrawSlider1(const char* label, float& v, const float& moveSpeed = 1.f);
	static void DrawSlider2(const char* label, Vec2& v, const float& moveSpeed = 1.f);
	static void DrawSlider3(const char* label, Vec3& v, const float& moveSpeed = 1.f);
	static void DrawColorEdit(const char* label, Color& color);
};