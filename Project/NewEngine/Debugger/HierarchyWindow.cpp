#include "MainWindow.h"
#include "HierarchyWindow.h"
#include "SceneManager.h"
#include "GameObjectManager.h"

void HierarchyWindow::DrawGuiWindow()
{
	Gui::BeginWindow("Hierarchy");

	CreateGameObjectPop();

	std::string label = SceneManager::GetInstance()->mCurrentScene->GetName();
	if (Gui::DrawCollapsingHeader(label.c_str(), true))
	{
		for (const auto& obj : *GetGameObjects())
		{
			if (Gui::BeginTreeNode(obj->name))
			{

				Gui::EndTreeNode();
			}

			// ツリーノードがクリックされたかどうかを検出
			if (ImGui::IsItemClicked())
			{
				MainWindow::GetInstance()->SetCurrentObjName(obj->name);
			}

			std::string& dragDropGameObjName = MainWindow::GetInstance()->mDragDropGameObjName;
			if (Gui::DragDropSource("DragDrop GameObject", obj->name))
			{
				dragDropGameObjName = obj->name;
			}

			if (Gui::DragDropTarget("DragDrop GameObject"))
			{
				if (!dragDropGameObjName.empty())
				{
					GameObject* child = GetGameObject(dragDropGameObjName);
					child->SetParent(obj->GetTransform());
					dragDropGameObjName.clear();
				}
			}
		}
	}

	if (ImGui::IsWindowHovered())
	{
		if (Key::GetKeyDown(DIK_DELETE))
		{
			SceneManager::AddPostDrawProcessFunc([]()
				{
					DestroyGameObject(MainWindow::GetInstance()->GetCurrentObjName());
				});
		}
	}

	Gui::EndWindow();
}

void HierarchyWindow::CreateGameObjectPop()
{
	// ウィンドウ内でのマウス右クリックの判定
	if (ImGui::IsWindowHovered() &&
		ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("Create GameObjcet");
	}

	if (ImGui::BeginPopup("Create GameObjcet"))
	{
		if (Gui::BeginMenu("3D Object"))
		{
			// オブジェクト3D
			ShowObjectMenuItem("Object3D", GameObjectType::Object3D);

			// パーティクルメッシュ
			ShowObjectMenuItem("Particle Mesh", GameObjectType::ParticleMesh, false);

			Gui::EndMenu();
		}
		if (Gui::BeginMenu("2D Object"))
		{
			// スプライト
			ShowObjectMenuItem("Sprite", GameObjectType::Sprite, false);

			Gui::EndMenu();
		}
		if (Gui::BeginMenu("Light"))
		{
			ShowObjectMenuItem("Directional Light", GameObjectType::DirectionalLight);
			ShowObjectMenuItem("Point Light", GameObjectType::PointLight);
			ShowObjectMenuItem("Spot Light", GameObjectType::SpotLight);

			Gui::EndMenu();
		}

		// カメラ
		ShowObjectMenuItem("Camera", GameObjectType::Camera, false);

		// EmptyObject
		ShowObjectMenuItem("Empty Object", GameObjectType::EmptyObject, false);

		ImGui::EndPopup();
	}
}

void HierarchyWindow::ShowObjectMenuItem(const std::string& label, const GameObjectType type, const bool isDrawLine)
{
	if (Gui::MenuItem(label))
	{
		uint32_t mapSize = (uint32_t)GetGameObjects()->size();
		std::string name = label + " " + std::to_string(mapSize);
		AddGameObject(type, name);
	}

	if (isDrawLine)
	{
		Gui::DrawLine();
	}
}
