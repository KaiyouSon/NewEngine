#pragma once
#include "imgui.h"
#include "Util.h"
#include "Texture.h"
#include "Singleton.h"
#include <stdint.h>

// ImGuiのラッピング関数
class GuiWraper : public Singleton<GuiWraper>
{
private:
	static const uint32_t sNumFramesInFlight = 3;
	bool isOpenGuiSetting;
	Color windowBgColor;
	Color menuBarBgColor;
	Color titleBgActiveColor;
	Color titleBgCollapsedColor;

private:
	ImVec4 ToImVec4(const Color color);

public:
	static void Init();
	static void PreDraw();
	static void PostDraw();
	static void Destroy();

	static void DrawGuiSetting();

public:
	static bool BeginWindow(const char* name, const Vec2& size = -1, bool* isOpen = nullptr);
	static void EndWindow();
	static void BeginFullWindow(const char* name);
	static void EndFullWindow();

	static void OpenPopModal(const char* tag);
	static void ClosePopModal();
	static bool BeginPopModal(const char* tag);
	static void EndPopModal();

	static bool DrawCollapsingHeader(const char* name);

	// メニューバー関連
	static bool BeginMenuBar();
	static void EndMenuBar();
	static bool BeginMenu(const char* name);
	static void EndMenu();
	static bool MenuItem(const char* name);

	static void DrawDemoWindow(bool& flag);

	static void DrawTab();
	static void DrawLine();
	static void DrawColumns(uint32_t space = 1, const bool& isBorder = false);
	static void NextColumn();

	// 文字列
	static void DrawString(const char* fmt, ...);

	// チェックボックス
	static void DrawCheckBox(const char* label, bool* flag);

	// ボタン関連
	static bool DrawButton(const char* label, const Vec2& size);
	static bool DrawRadioButton(const char* label, uint32_t* current, const uint32_t index, const bool isTab = true);
	static bool DrawImageButton(ITexture* texture, const Vec2& size);

	// スライダー描画
	static void DrawSlider1(const char* label, float& v, const float& moveSpeed = 1.f);
	static void DrawSlider2(const char* label, Vec2& v, const float& moveSpeed = 1.f);
	static void DrawSlider3(const char* label, Vec3& v, const float& moveSpeed = 1.f);

	// カラーエディター描画
	static void DrawColorEdit(const char* label, Color& color);

	// 入力関連
	static bool DrawInputInt(const char* label, int32_t& v);
	static bool DrawInputText(const char* label, std::string& str);

	// 画像関連
	static void DrawImage(ITexture* texture, const Vec2& size);
	static void DrawImage(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, const Vec2& size);

private:
	GuiWraper();
	friend Singleton<GuiWraper>;

}typedef Gui;
