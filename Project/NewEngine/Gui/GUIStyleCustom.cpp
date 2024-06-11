#include "stdafx.h"
#include "GUIStyleCustom.h"
#include <imgui.h>

void GUIStyleCustom::SetToDarkStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	ImVec4* colors = style->Colors;

	ImVec4 borderColor = ImVec4(0.01f, 0.01f, 0.1f, 1.0f);

	ImVec4 textColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	ImVec4 bg1Color = ImVec4(0.01f, 0.01f, 0.01f, 1.0f);

	ImVec4 windowBackgroundColor = ImVec4(0.01f, 0.01f, 0.01f, 1.0f);
	ImVec4 frameBackgroundColor = ImVec4(0.016f, 0.016f, 0.016f, 1.00f);
	ImVec4 frameBackgroundHoveredColor = ImVec4(0.024f, 0.024f, 0.024f, 1.00f);
	ImVec4 titleBackgroundColor = ImVec4(0.006f, 0.006f, 0.006f, 1.00f);
	ImVec4 titleBackgroundActiveColor = ImVec4(0.006f*2, 0.006f * 2, 0.006f * 2, 1.00f);
	ImVec4 inactiveTitleBackgroundColor = ImVec4(0.026f, 0.026f, 0.026f, 1.00f);
	ImVec4 separatorColor = ImVec4(0.024f, 0.024f, 0.024f, 1.00f);
	ImVec4 gripColor = ImVec4(0.026f, 0.059f, 0.098f, 1.00f);

	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = windowBackgroundColor;
	colors[ImGuiCol_ChildBg] = windowBackgroundColor;
	colors[ImGuiCol_PopupBg] = windowBackgroundColor;
	colors[ImGuiCol_Border] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = frameBackgroundColor;
	colors[ImGuiCol_FrameBgHovered] = frameBackgroundHoveredColor;
	colors[ImGuiCol_FrameBgActive] = frameBackgroundHoveredColor;
	colors[ImGuiCol_TitleBg] = titleBackgroundColor;
	colors[ImGuiCol_TitleBgActive] = titleBackgroundActiveColor;
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.010f, 0.010f, 0.010f, 1.f);
	colors[ImGuiCol_MenuBarBg] = windowBackgroundColor;
	colors[ImGuiCol_ScrollbarBg] = windowBackgroundColor;
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_CheckMark] = titleBackgroundColor;
	colors[ImGuiCol_SliderGrab] = titleBackgroundColor;
	colors[ImGuiCol_SliderGrabActive] = titleBackgroundColor;
	colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = frameBackgroundHoveredColor;
	colors[ImGuiCol_ButtonActive] = inactiveTitleBackgroundColor;
	colors[ImGuiCol_Header] = frameBackgroundHoveredColor;
	colors[ImGuiCol_HeaderHovered] = inactiveTitleBackgroundColor;
	colors[ImGuiCol_HeaderActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	colors[ImGuiCol_Separator] = separatorColor;
	colors[ImGuiCol_SeparatorHovered] = separatorColor;
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = gripColor;
	colors[ImGuiCol_ResizeGripHovered] = gripColor;
	colors[ImGuiCol_ResizeGripActive] = gripColor;
	colors[ImGuiCol_Tab] = frameBackgroundColor;
	colors[ImGuiCol_TabHovered] = frameBackgroundHoveredColor;
	colors[ImGuiCol_TabActive] = frameBackgroundHoveredColor;
	colors[ImGuiCol_TabUnfocused] = frameBackgroundColor;
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = frameBackgroundColor;
	colors[ImGuiCol_TableBorderStrong] = frameBackgroundColor;
	colors[ImGuiCol_TableBorderLight] = frameBackgroundColor;
	colors[ImGuiCol_TableRowBg] = windowBackgroundColor;
	colors[ImGuiCol_TableRowBgAlt] = windowBackgroundColor;
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = titleBackgroundColor;
	colors[ImGuiCol_NavHighlight] = titleBackgroundColor;
	colors[ImGuiCol_NavWindowingHighlight] = titleBackgroundColor;
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImGui::GetIO().FontGlobalScale = 1.1f;
	style->FrameRounding = 12.f;
}
