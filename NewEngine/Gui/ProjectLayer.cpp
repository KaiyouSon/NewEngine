#include "ProjectLayer.h"
#include "DrawManager.h"
#include <processthreadsapi.h>
using namespace std::filesystem;

//Texture* cppTex;
//Texture* hTex;
//Texture* objTex;
//Texture* folderTex;

//ProjectLayer::~ProjectLayer()
//{
//	UnLoadTexture(cppTex);
//	UnLoadTexture(hTex);
//	UnLoadTexture(objTex);
//	UnLoadTexture(folderTex);
//}

void ProjectLayer::Initialize()
{
	size = { 1440,340 };
	pos = { 0,WIN_HEIGHT - size.y };
	//cppTex = LoadTexture("NewEngine/Resources/icon/cppFileTex.png");
	//hTex = LoadTexture("NewEngine/Resources/icon/hFileTex.png");
	//objTex = LoadTexture("NewEngine/Resources/icon/objFileTex.png");
	//folderTex = LoadTexture("NewEngine/Resources/icon/folderTex.png");

	projectDirectroy = "GameProject";	// プロジェクトディレクトリー
	currentDirectroy = projectDirectroy;
}
void ProjectLayer::Update()
{
	//ImGuiWindowFlags window_flags = 0;
	//window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
	////ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
	////ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	//ImGui::Begin("Project", nullptr, window_flags);

	//static float pading = 16;		// 間隔の幅
	//static float buttonSize = 128;	// ボタンの大きさ
	//float cellSize = buttonSize + pading;	// セル何個分？
	//float panelWidth = ImGui::GetContentRegionAvail().x;
	//int padingCount = panelWidth / cellSize;
	//if (padingCount < 1) padingCount = 1;

	//if (ImGui::BeginMenuBar()) {
	//	if (ImGui::BeginMenu("Setting"))
	//	{
	//		ImGui::SliderFloat("Button Size", &buttonSize, 32, 256);
	//		ImGui::SliderFloat("Pading", &pading, 0, 32);
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//}

	//if (currentDirectroy != path(projectDirectroy))
	//{
	//	if (ImGui::Button("<-"))
	//	{
	//		currentDirectroy = currentDirectroy.parent_path();
	//	}
	//}

	//ImGui::Columns(padingCount, 0, false);

	//// 現在のディレクトリ内のファイルを見る
	//for (auto& directroy : directory_iterator(currentDirectroy))
	//{
	//	const auto& path = directroy.path();					// ディレクトリーのパス取得
	//	auto relativePath = relative(path, projectDirectroy);	// 相対パス
	//	std::string filenameString = relativePath.filename().string();	// ファイルの名前文字列の変換

	//	if (directroy.is_directory())
	//	{
	//		//if (ImGui::ImageButton((ImTextureID)folderTex.GetGpuHandle().ptr, { buttonSize,buttonSize }))
	//		//{
	//		//	currentDirectroy /= path.filename();
	//		//}

	//		// フォルダー
	//		if (ImGui::Button(filenameString.c_str(), { buttonSize,buttonSize }))
	//		{
	//			currentDirectroy /= path.filename();
	//		}
	//		ImGui::Text(filenameString.c_str());
	//	}
	//	else
	//	{
	//		// cppファイル
	//		if (filenameString.substr(filenameString.length() - 4) == ".cpp")
	//		{
	//			ImGui::ImageButton((ImTextureID)cppTex->GetGpuHandle().ptr, { buttonSize,buttonSize });
	//			ImGui::Text(filenameString.c_str());
	//		}
	//		// hファイル
	//		else if (filenameString.substr(filenameString.length() - 2) == ".h")
	//		{
	//			ImGui::ImageButton((ImTextureID)hTex->GetGpuHandle().ptr, { buttonSize,buttonSize });
	//			ImGui::Text(filenameString.c_str());
	//		}
	//		// objファイル
	//		else if (filenameString.substr(filenameString.length() - 4) == ".obj")
	//		{
	//			ImGui::ImageButton((ImTextureID)objTex->GetGpuHandle().ptr, { buttonSize,buttonSize });
	//			ImGui::Text(filenameString.c_str());
	//		}
	//		else
	//		{
	//			ImGui::Button(filenameString.c_str(), { buttonSize,buttonSize });
	//			ImGui::Text(filenameString.c_str());
	//		}
	//	}
	//	ImGui::NextColumn();
	//}
	//ImGui::Columns(1);

	//ImGui::End();
}