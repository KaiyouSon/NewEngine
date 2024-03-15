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
			if (obj->GetParent())
			{
				continue;
			}

			// 再帰で親子関係を確認できるようにしてる
			RecursiveShowObject(obj.get());
		}
	}

	if (isDrag)
	{
		Gui::DrawLine();

		ImGui::PushStyleColor(ImGuiCol_Button, Gui::ToImVec4(Color::black + 25));
		float width = ImGui::GetContentRegionAvail().x;
		if (Gui::DrawButton("Parent-Child Cancel", Vec2(width, 32)))
		{
			mDragDropSourceObjName.clear();
			isDrag = false;
		}
		ImGui::PopStyleColor();

		if (Gui::DragDropTarget("DragDrop GameObject"))
		{
			if (!mDragDropSourceObjName.empty())
			{
				GameObject* child = GetGameObject(mDragDropSourceObjName);
				child->SetParent(nullptr);

				mDragDropSourceObjName.clear();
				isDrag = false;
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

void HierarchyWindow::RecursiveShowObject(GameObject* obj)
{
	bool isNodeOpen = Gui::BeginTreeNode(obj->name);

	if (ImGui::IsItemClicked())
	{
		MainWindow::GetInstance()->SetCurrentObjName(obj->name);
	}

	DragAndDropObject(obj);

	if (isNodeOpen)
	{
		// 子オブジェクトの描画
		for (const auto& child : *GetGameObjects())
		{
			if (child->GetParent() == obj->GetTransform())
			{
				RecursiveShowObject(child.get());
			}
		}
		Gui::EndTreeNode();
	}
}

void HierarchyWindow::DragAndDropObject(GameObject* obj)
{
	if (Gui::DragDropSource("DragDrop GameObject", obj->name))
	{
		mDragDropSourceObjName = obj->name;
		isDrag = true;
	}

	if (!mDragDropSourceObjName.empty())
	{
		if (Gui::DragDropTarget("DragDrop GameObject"))
		{
			GameObject* child = GetGameObject(mDragDropSourceObjName);
			child->SetParent(obj->GetTransform());

			mDragDropSourceObjName.clear();
			isDrag = false;
		}
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

