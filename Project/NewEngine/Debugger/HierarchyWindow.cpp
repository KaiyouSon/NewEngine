#include "MainWindow.h"
#include "HierarchyWindow.h"
#include "SceneManager.h"
#include "GameObjectManager.h"

void HierarchyWindow::DrawGuiWindow()
{
	Gui::BeginWindow("Hierarchy");

	CreateGameObjectPop();

	std::string label = SceneManager::GetInstance()->mCurrentScene->GetTag();
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
		uint32_t mapSize = (uint32_t)GetGameObjects()->size();
		std::string name;

		// オブジェクトなら
		if (Gui::MenuItem("Object3D"))
		{
			name = "Object" + std::to_string(mapSize);
			AddGameObject(GameObjectType::Object3D, name);
		}
		Gui::DrawLine();

		// スプライトなら
		if (Gui::MenuItem("Sprite"))
		{
			name = "Sprite" + std::to_string(mapSize);
			AddGameObject(GameObjectType::Sprite, name);
		}
		Gui::DrawLine();

		ImGui::EndPopup();
	}
}