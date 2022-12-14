#include "GuiManager.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "Util.h"
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

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

bool GuiManager::BeginWindow()
{
	return ImGui::Begin("Debug");
}

void GuiManager::EndWindow()
{
	ImGui::End();
}

void GuiManager::DrawString(const char* fmt, ...)
{
	ImGui::Text(fmt);
}

void GuiManager::DrawCheckBox(const char* label, bool* flag)
{
	ImGui::Checkbox(label, flag);
}

void GuiManager::DrawSlider(const char* label, Vec3& v)
{
	float temp[3] = { v.x,v.y,v.z };

	ImGui::DragFloat3(label, temp);

	v.x = temp[0];
	v.y = temp[1];
	v.z = temp[2];
}
