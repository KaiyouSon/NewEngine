#pragma once
#include "imgui.h"
#include "Util.h"
#include "Texture.h"

template<typename T> class Singleton;

class GuiManager : public Singleton<GuiManager>
{
private:
	static const uint32_t sNumFramesInFlight;

public:
	void Init();
	void PreDraw();
	void PostDraw();
	void Destroy();

	static bool BeginWindow(const char* name, const Vec2& size = -1);
	static void EndWindow();
	static void BeginFullWindow(const char* name);
	static void EndFullWindow();

	static void OpenPopModal(const char* tag);
	static void ClosePopModal();
	static bool BeginPopModal(const char* tag);
	static void EndPopModal();

	static void DrawDemoWindow(bool& flag);

	static bool DrawButton(const char* label, const Vec2& size);

	static void DrawTab();
	static void DrawLine();

	static void DrawColumns(uint32_t space = 1, const bool& isBorder = false);
	static void NextColumn();

	static void DrawString(const char* fmt, ...);
	static void DrawCheckBox(const char* label, bool* flag);

	static bool DrawRadioButton(const char* label, int& current, const int& index, const bool& isTab = true);

	static void DrawSlider1(const char* label, float& v, const float& moveSpeed = 1.f);
	static void DrawSlider2(const char* label, Vec2& v, const float& moveSpeed = 1.f);
	static void DrawSlider3(const char* label, Vec3& v, const float& moveSpeed = 1.f);
	static void DrawColorEdit(const char* label, Color& color);

	static bool DrawInputInt(const char* label, int& v);
	static bool DrawInputText(const char* label, const std::string& str);

	static void DrawImage(Texture* texture, const Vec2& size);
	static bool DrawImageButton(Texture* texture, const Vec2& size);


private:
	friend Singleton<GuiManager>;
	GuiManager() {};
};