#pragma once
#include "imgui.h"
#include "Util.h"
#include "Texture.h"
#include <stdint.h>

namespace Gui
{
	static const uint32_t sNumFramesInFlight = 3;;

	void Init();
	void PreDraw();
	void PostDraw();
	void Destroy();

	bool BeginWindow(const char* name, const Vec2& size = -1, bool* isOpen = nullptr);
	void EndWindow();
	void BeginFullWindow(const char* name);
	void EndFullWindow();

	void OpenPopModal(const char* tag);
	void ClosePopModal();
	bool BeginPopModal(const char* tag);
	void EndPopModal();

	bool DrawCollapsingHeader(const char* name);

	// 繝｡繝九Η繝ｼ髢｢騾｣
	bool BeginMenuBar();
	void EndMenuBar();
	bool BeginMenu(const char* name);
	void EndMenu();
	bool MenuItem(const char* name);

	void DrawDemoWindow(bool& flag);

	bool DrawButton(const char* label, const Vec2& size);

	void DrawTab();
	void DrawLine();

	void DrawColumns(uint32_t space = 1, const bool& isBorder = false);
	void NextColumn();

	void DrawString(const char* fmt, ...);
	void DrawCheckBox(const char* label, bool* flag);

	bool DrawRadioButton(const char* label, uint32_t* current, const uint32_t index, const bool isTab = true);

	void DrawSlider1(const char* label, float& v, const float& moveSpeed = 1.f);
	void DrawSlider2(const char* label, Vec2& v, const float& moveSpeed = 1.f);
	void DrawSlider3(const char* label, Vec3& v, const float& moveSpeed = 1.f);
	void DrawColorEdit(const char* label, Color& color);

	bool DrawInputInt(const char* label, int& v);
	bool DrawInputText(const char* label, const std::string& str);

	void DrawImage(Texture* texture, const Vec2& size);
	void DrawImage(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, const Vec2& size);
	bool DrawImageButton(Texture* texture, const Vec2& size);
};
