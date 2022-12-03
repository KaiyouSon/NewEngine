#include "MainLayer.h"
#include "GuiManager.h"

void MainLayer::Initialize()
{
	size = Vec2(WIN_WIDTH, WIN_HEIGHT);
	pos = Vec2(0, 0);
	isShowConsoleWindow = false;
}
void MainLayer::Update()
{
	static bool dockspaceOpen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoTitleBar |				// タイトルバー表示しない
		ImGuiWindowFlags_NoResize |					// サイズ変更しない
		ImGuiWindowFlags_NoMove |					// 座標変更しない
		ImGuiWindowFlags_MenuBar |					// メニューバー表示
		ImGuiWindowFlags_NoBringToFrontOnFocus |	// クリックしたら最前面に表示しない
		ImGuiWindowFlags_NoCollapse;				// 折り畳みしない
		//ImGuiWindowFlags_NoNavFocus;

	// ウィンドウの設定
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	//ShowMenuBar();

	ImGui::End();
	ImGui::PopStyleVar();
}

void MainLayer::ShowMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) {}
			ImGui::Separator();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Console", nullptr, &isShowConsoleWindow);
			ImGui::Separator();

			if (ImGui::MenuItem("Debug Window")) {}
			ImGui::Separator();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//if (isShowConsoleWindow == true) ShowConsoleWindow();
}
void MainLayer::ShowConsoleWindow()
{
	if (ImGui::Begin("Console", &isShowConsoleWindow, ImGuiWindowFlags_NoCollapse))
	{
		for (int i = 0; i < textList.size(); i++)
		{
			ImGui::Text("%d:", i + 1);
			ImGui::SameLine();
			ImGui::Text(textList[i].c_str());
			ImGui::Separator();
		}
		ImGui::End();
	}
}

void MainLayer::AddText(const std::string& text)
{
	textList.push_back(text);
}
