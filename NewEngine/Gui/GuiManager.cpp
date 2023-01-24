#include "GuiManager.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "Util.h"
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

const int GuiManager::NumFramesInFlight = 3;

void GuiManager::Init()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(RenderWindow::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		renderBase->GetDevice(),
		NumFramesInFlight,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		renderBase->GetSrvDescHeap(),
		renderBase->GetSrvDescHeap()->GetCPUDescriptorHandleForHeapStart(),
		renderBase->GetSrvDescHeap()->GetGPUDescriptorHandleForHeapStart());
}

void GuiManager::PreDraw()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::NewFrame();
}

void GuiManager::PostDraw()
{
	ImGui::Render();
	// SRVヒープの設定コマンド
	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	auto srvDescHeap = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &srvDescHeap);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderBase->GetCommandList());
}

void GuiManager::Destroy()
{
	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool GuiManager::BeginWindow(const char* name)
{
	return ImGui::Begin(name);
}

void GuiManager::EndWindow()
{
	ImGui::End();
}

void GuiManager::DrawDebugWindow(bool& flag)
{
	ImGui::ShowDemoWindow(&flag);
}

void GuiManager::DrawTab()
{
	ImGui::SameLine();
}

void GuiManager::DrawString(const char* fmt, ...)
{
	ImGui::Text(fmt);
}

void GuiManager::DrawCheckBox(const char* label, bool* flag)
{
	ImGui::Checkbox(label, flag);
}

bool GuiManager::DrawRadioButton(const char* label, int& current, const int& index, const bool& isTab)
{
	bool flag = ImGui::RadioButton(label, &current, index);
	if (isTab == true)
	{
		ImGui::SameLine();
	}

	return flag;
}

void GuiManager::DrawSlider1(const char* label, float& v, const float& moveSpeed)
{
	ImGui::DragFloat(label, &v, moveSpeed);
}

void GuiManager::DrawSlider2(const char* label, Vec2& v, const float& moveSpeed)
{
	float temp[2] = { v.x,v.y };
	ImGui::DragFloat2(label, temp, moveSpeed);
	v.x = temp[0]; v.y = temp[1];
}

void GuiManager::DrawSlider3(const char* label, Vec3& v, const float& moveSpeed)
{
	float temp[3] = { v.x,v.y,v.z };
	ImGui::DragFloat3(label, temp, moveSpeed);
	v.x = temp[0];	v.y = temp[1];	v.z = temp[2];
}

void GuiManager::DrawColorEdit(const char* label, Color& color)
{
	float temp[4] = { color.r / 255,color.g / 255,color.b / 255,color.a / 255 };
	ImGui::ColorEdit4(label, temp);
	color.r = temp[0] * 255; color.g = temp[1] * 255; color.b = temp[2] * 255; color.a = temp[3] * 255;
}
