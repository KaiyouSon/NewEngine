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

	projectDirectroy = "GameProject";	// �v���W�F�N�g�f�B���N�g���[
	currentDirectroy = projectDirectroy;
}
void ProjectLayer::Update()
{
	//ImGuiWindowFlags window_flags = 0;
	//window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
	////ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
	////ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	//ImGui::Begin("Project", nullptr, window_flags);

	//static float pading = 16;		// �Ԋu�̕�
	//static float buttonSize = 128;	// �{�^���̑傫��
	//float cellSize = buttonSize + pading;	// �Z�������H
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

	//// ���݂̃f�B���N�g�����̃t�@�C��������
	//for (auto& directroy : directory_iterator(currentDirectroy))
	//{
	//	const auto& path = directroy.path();					// �f�B���N�g���[�̃p�X�擾
	//	auto relativePath = relative(path, projectDirectroy);	// ���΃p�X
	//	std::string filenameString = relativePath.filename().string();	// �t�@�C���̖��O������̕ϊ�

	//	if (directroy.is_directory())
	//	{
	//		//if (ImGui::ImageButton((ImTextureID)folderTex.GetGpuHandle().ptr, { buttonSize,buttonSize }))
	//		//{
	//		//	currentDirectroy /= path.filename();
	//		//}

	//		// �t�H���_�[
	//		if (ImGui::Button(filenameString.c_str(), { buttonSize,buttonSize }))
	//		{
	//			currentDirectroy /= path.filename();
	//		}
	//		ImGui::Text(filenameString.c_str());
	//	}
	//	else
	//	{
	//		// cpp�t�@�C��
	//		if (filenameString.substr(filenameString.length() - 4) == ".cpp")
	//		{
	//			ImGui::ImageButton((ImTextureID)cppTex->GetGpuHandle().ptr, { buttonSize,buttonSize });
	//			ImGui::Text(filenameString.c_str());
	//		}
	//		// h�t�@�C��
	//		else if (filenameString.substr(filenameString.length() - 2) == ".h")
	//		{
	//			ImGui::ImageButton((ImTextureID)hTex->GetGpuHandle().ptr, { buttonSize,buttonSize });
	//			ImGui::Text(filenameString.c_str());
	//		}
	//		// obj�t�@�C��
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