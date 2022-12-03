#include "GuiManager.h"
#include "RenderWindow.h"
#include "Util.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx12.h"
#include "ImGUI/imgui_impl_win32.h"

const int GuiManager::NumFramesInFlight = 3;

void GuiManager::Initialize()
{
	renderBase = RenderBase::GetInstance().get();
	mainLayer = MainLayer::GetInstance().get();
	sceneLayer = SceneLayer::GetInstance().get();
	projectLayer = ProjectLayer::GetInstance().get();
	hierarchyLayer = HierarchyLayer::GetInstance().get();
	userLayer = UserLayer::GetInstance().get();
	inspectorLayer = InspectorLayer::GetInstance().get();

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

	mainLayer->Initialize();
	sceneLayer->Initialize();
	projectLayer->Initialize();
	hierarchyLayer->Initialize();
	userLayer->Initialize();
	inspectorLayer->Initialize();
}

void GuiManager::Update()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.DisplaySize.x = 1920;
	//io.DisplaySize.y = 1010;

	ImGui::NewFrame();

	ImGui::GetMainViewport();
	ImGui::PushStyleColor(ImGuiCol_TitleBg, { 0.05f, 0.05f, 0.05f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.1f, 0.1f, 0.1f, 1.0f });
	mainLayer->Update();
	sceneLayer->Update();
	projectLayer->Update();
	hierarchyLayer->Update();
	userLayer->Update();
	inspectorLayer->Update();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//bool show_demo_window = true;
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);
}

void GuiManager::Draw()
{
	ImGui::Render();
	// SRVヒープの設定コマンド
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderBase->GetCommandList());
}

void GuiManager::Destroy()
{
	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}