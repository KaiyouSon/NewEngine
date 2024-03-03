#include "AssetsWindow.h"
#include "MainWindow.h"
#include "ModelManager.h"
#include "SceneManager.h"
#include "StandardLib.h"

namespace fs = std::filesystem;

AssetsWindow::AssetsWindow() :
	state(MainLevel),
	padding(16.f), buttonSize(128.f), columnCount(0)
{
}

void AssetsWindow::DrawGuiWindow()
{
	Gui::BeginWindow("Assets");

	mIsMouseInWindow = ImGui::IsWindowHovered();

	CreateAssetsPop();

	// ファイルパネル
	float cellSize = buttonSize + padding;
	float panelWidth = ImGui::GetContentRegionAvail().x;
	columnCount = Max<int>(1, (int)(panelWidth / cellSize));

	Gui::BeginMenuBar();
	Gui::DrawColumns(3);
	Gui::DrawSlider1("Padding", padding, 1, 64);
	padding = Max(0.0f, padding);

	Gui::DrawSlider1("Button Size", buttonSize, 1, 64);
	buttonSize = Max(16.f, buttonSize);
	Gui::NextColumn(2);

	Gui::DrawInputText("Search", search, 256);
	Gui::DrawColumns(1);

	Gui::EndMenuBar();

	switch (state)
	{
	case MainLevel:
		ShowMainLevel();
		break;

	case SceneAssets:
		ShowSceneAssets();
		break;

	case TextureAssets:
		ShowTextureAssets();
		break;

	case ModelAssets:
		ShowModelAssets();
		break;

	case MaterialAssets:
		ShowMaterialAssets();
	}

	Gui::EndWindow();
}

bool AssetsWindow::GetMouseInWindow()
{
	AssetsWindow temp;
	return temp.mIsMouseInWindow;
}

void AssetsWindow::ShowMainLevel()
{
	Gui::DrawColumns(columnCount);

	if (Gui::DrawButton("Scene", buttonSize))
	{
		state = SceneAssets;
	}
	Gui::NextColumn();

	if (Gui::DrawButton("Texture", buttonSize))
	{
		state = TextureAssets;
	}
	Gui::NextColumn();

	if (Gui::DrawButton("Model", buttonSize))
	{
		state = ModelAssets;
	}
	Gui::NextColumn();

	if (Gui::DrawButton("Material", buttonSize))
	{
		state = MaterialAssets;
	}

	Gui::DrawColumns(1);
}
void AssetsWindow::ShowSceneAssets()
{
	DrawBackButton();

	fs::path folderPath = EngineDataDirectory + "Scene";// +SceneManager::GetInstance()->mCurrentScene->GetName() + "Scene.json";
	for (const auto entry : fs::directory_iterator(folderPath))
	{
		const fs::path path = entry.path();
		const fs::path relativePath = relative(path, folderPath);	// 相対パス
		const std::string filename = relativePath.filename().string();
		const std::string sceneName = SubString(filename, "Scene.json");

		// 検索機能
		std::string::size_type pos = sceneName.find(search);
		if (pos == std::string::npos)
		{
			continue;
		}

		Gui::DrawColumns(columnCount);
		Gui::DrawButton(sceneName.c_str(), buttonSize);
		Gui::NextColumn();

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			static std::string selectSceneName;
			selectSceneName = sceneName;
			SceneManager::ChangeScene(selectSceneName);
		}
	}
}
void AssetsWindow::ShowTextureAssets()
{
	DrawBackButton();

	int count = 1;
	for (const auto& [tag, tex] : *gAssetsManager->GetTextureMap(TextureType::Default))
	{
		if (!tex)
		{
			continue;
		}

		// 検索機能
		std::string::size_type pos = tag.find(search);
		if (pos == std::string::npos)
		{
			continue;
		}

		Gui::DrawColumns(columnCount);
		Gui::DrawString("Tag  : %s", tag.c_str());
		Gui::DrawString("Size : %dx%d", (uint32_t)tex->GetInitalSize().x, (uint32_t)tex->GetInitalSize().y);

		Gui::DrawImageButton(tex.get(), buttonSize);

		if (Gui::DragDropSource("DragDrop Texture", tag))
		{
			MainWindow::GetInstance()->SetDragDropAssetsTag(tag);
		}

		Gui::NextColumn();

		count++;
		if (count % columnCount == 0)
		{
			Gui::DrawLine();
		}
	}
	Gui::DrawColumns(1);
}
void AssetsWindow::ShowModelAssets()
{
	DrawBackButton();

	int count = 1;
	for (const auto& [tag, model] : *gAssetsManager->GetModelMap())
	{
		// 検索機能
		std::string::size_type pos = tag.find(search);
		if (pos == std::string::npos)
		{
			continue;
		}

		Gui::DrawColumns(columnCount);
		Gui::DrawString("Tag  : %s", tag.c_str());

		std::string formatStr = (model->format == ModelFormat::Obj) ? "obj" : "fbx";
		Gui::DrawString("Format : %s", formatStr.c_str());

		Gui::DrawButton(tag.c_str(), buttonSize);
		Gui::NextColumn();

		count++;
		if (count % columnCount == 0)
		{
			Gui::DrawLine();
		}
	}
	Gui::DrawColumns(1);
}
void AssetsWindow::ShowMaterialAssets()
{
	DrawBackButton();

	fs::path folderPath = EngineMaterialDirectory;
	for (const auto entry : fs::directory_iterator(folderPath))
	{
		const fs::path path = entry.path();
		const fs::path relativePath = relative(path, folderPath);	// 相対パス
		const std::string filename = relativePath.filename().string();
		const std::string sceneName = SubString(filename, "Material.json");

		// 検索機能
		std::string::size_type pos = sceneName.find(search);
		if (pos == std::string::npos)
		{
			continue;
		}

		Gui::DrawColumns(columnCount);
		Gui::DrawButton(sceneName.c_str(), buttonSize);
		Gui::NextColumn();

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{

		}
	}
}

void AssetsWindow::DrawBackButton()
{
	if (Gui::DrawButton("<- Back", Vec2(64, 24)))
	{
		state = MainLevel;
	}
}

void AssetsWindow::CreateAssetsPop()
{
	// ウィンドウ内でのマウス右クリックの判定
	if (ImGui::IsWindowHovered() &&
		ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("Create Assets");
	}

	static bool mIsOpenCreateScenePopModel = false;
	if (ImGui::BeginPopup("Create Assets"))
	{
		// オブジェクトなら
		if (Gui::MenuItem("Scene"))
		{
			mIsOpenCreateScenePopModel = true;
		}
		Gui::DrawLine();

		ImGui::EndPopup();
	}

	if (mIsOpenCreateScenePopModel)
	{
		Gui::OpenPopModal("Create Scene");
	}

	int32_t flag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	if (ImGui::BeginPopupModal("Create Scene", &mIsOpenCreateScenePopModel, flag))
	{
		ImVec2 size = ImVec2(320, 80);

		ImGui::SetWindowPos(ImVec2(GetWindowHalfSize().x - size.x / 2, GetWindowHalfSize().y - size.y / 2));
		ImGui::SetWindowSize(size);

		static std::string sceneTag;
		Gui::DrawInputText("Scene Tag", sceneTag);

		if (Gui::DrawButton("create", Vec2(48, 24)))
		{
			SceneManager::CreateScene(sceneTag);
			mIsOpenCreateScenePopModel = false;
			Gui::ClosePopModal();
		}
		Gui::DrawTab();
		if (Gui::DrawButton("cancel", Vec2(48, 24)))
		{
			mIsOpenCreateScenePopModel = false;
			Gui::ClosePopModal();
		}

		ImGui::EndPopup();
	}

}
