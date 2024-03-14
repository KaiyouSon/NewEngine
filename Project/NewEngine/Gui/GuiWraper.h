#pragma once
#include "imgui.h"
#include "Util.h"
#include "Texture.h"
#include "Singleton.h"
#include "StandardLib.h"

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

public:
	static ImVec4 ToImVec4(const Color color);
	static Vec2 ToVec2(const ImVec2 v);

public:
	// レイアウト関連
	static void SetNextItemToCenterWidth(const float itemWidth);

public:
	static void Init();
	static void PreDraw();
	static void PostDraw();
	static void Destroy();

	static void DrawGuiSetting();

public:
	static bool BeginWindow(const char* name, const Vec2& size = -1, bool* isOpen = nullptr, bool isUseMenuBar = true);
	static void EndWindow();
	static void BeginFullWindow(const char* name);
	static void EndFullWindow();

	static void OpenPopModal(const char* tag);
	static void ClosePopModal();
	static bool BeginPopModal(const char* tag);
	static void EndPopModal();

	static bool BeginTreeNode(const std::string& tag, const bool isOpenNode = false);
	static void EndTreeNode();

	static bool DrawCollapsingHeader(const char* name, const bool isOpenNode = false, bool* isCloseButton = nullptr);

public:
	// メニューバー関連
	static void DrawMainMenuBar(const std::function<void()>& menuBarFunc = nullptr);

	static bool BeginMenuBar();
	static void EndMenuBar();
	static bool BeginMenu(const char* name);
	static void EndMenu();
	static bool MenuItem(const std::string label, const std::string text = std::string(), bool selected = false);

	static void DrawDemoWindow(bool& flag);

	static void DrawTab();
	static void DrawLine();
	static void DrawColumns(const uint32_t column = 1, const bool& isBorder = false);
	static void NextColumn(const uint32_t column = 1);

	// 文字列
	static void DrawString(const char* fmt, ...);

	// チェックボックス
	static void DrawCheckBox(const char* label, bool* flag);

	// ボタン関連
	static bool DrawButton(const char* label, const Vec2& size);
	static bool DrawButtonToWidthCenter(const std::string& label, const Vec2& size);
	static bool DrawRadioButton(const char* label, uint32_t* current, const uint32_t index, const bool isTab = true);
	static bool DrawImageButton(ITexture* texture, const Vec2& size);

	// スライダー描画
	static void DrawSlider1(const std::string& label, float& v, const float& moveSpeed = 1.f, const float guiWidth = 0);
	static void DrawSlider2(const std::string& label, Vec2& v, const float& moveSpeed = 1.f);
	static void DrawSlider3(const std::string& label, Vec3& v, const float& moveSpeed = 1.f);
	static void DrawSlider4(const std::string& label, Vec4& v, const float& moveSpeed = 1.f);

	// カラーエディター描画
	static void DrawColorEdit(const char* label, Color& color);

	// 入力関連
	static bool DrawInputInt(const char* label, int32_t& v);
	static bool DrawInputInt(const char* label, uint32_t& v);
	static bool DrawInputText(const char* label, std::string& str, const float guiWidth = 0);

	// 画像関連
	static void DrawImage(ITexture* texture, const Vec2& size);
	static void DrawImage(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, const Vec2& size);

	// ドラッグ&ドロップ関連
	static bool DragDropSource(const std::string& label, const std::string& text);
	static bool DragDropTarget(const std::string& label);

public:
	/// <summary>
	/// std::unordered_map (std::string, T) 型のmapをImGui::Combo()で表示する関数のラッピング関数
	/// </summary>
	/// <param name="label : Comboのラベル"></param>
	/// <param name="previewLabel : 現在表示してるCombo要素のラベル"></param>
	/// <param name="currentComboIndex : 現在表示してるCombo要素のindex"></param>
	/// <param name="unorderedMap : std::unordered_map (std::string, T)型のmapデータ"></param>
	/// <param name="selecedFunc : 選択時のコールバック関数"></param>
	template<typename T>
	static void DrawItemsMapCombo(
		const std::string& label, std::string& previewLabel,
		uint32_t& currentComboIndex, const std::unordered_map<std::string, T>& unorderedMap,
		const std::function<void()>& selecedFunc = nullptr);

private:
	GuiWraper();
	friend Singleton<GuiWraper>;

}typedef Gui;

template<typename T>
inline void GuiWraper::DrawItemsMapCombo(
	const std::string& label, std::string& previewLabel,
	uint32_t& currentComboIndex, const std::unordered_map<std::string, T>& unorderedMap,
	const std::function<void()>& selecedFunc)
{
	if (ImGui::BeginCombo(label.c_str(), previewLabel.c_str()))
	{
		uint32_t index = 0;
		for (const auto& [tag, item] : unorderedMap)
		{
			const bool isSelected = (currentComboIndex == index);
			if (ImGui::Selectable(tag.c_str(), isSelected))
			{
				ImGui::SetItemDefaultFocus();
				currentComboIndex = index;
				previewLabel = tag;

				// 選択時に実行する関数
				selecedFunc();
			}
			index++;
		}
		ImGui::EndCombo();
	}
}
