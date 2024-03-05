#include "MainWindow.h"
#include "InspectorWindow.h"
#include "GameObjectManager.h"
#include "ModelManager.h"

void InspectorWindow::DrawGuiWindow()
{
	mCurrentObjTag = MainWindow::GetInstance()->GetCurrentObjName();
	mCurrentObj = GetGameObject(mCurrentObjTag);

	Gui::BeginWindow("Inspector");

	if (mCurrentObj)
	{
		// オブジェクトの情報
		ShowGameObjectData();

		// コンポネント追加ボタン
		AddComponetButton();
	}

	if (Key::GetKey(DIK_LCONTROL))
	{
		if (Key::GetKeyDown(DIK_C))
		{
			mCopyObj = gCurrentScene->GetGameObjectManager()->GetGameObject(mCurrentObjTag);
		}
		else if (Key::GetKeyDown(DIK_V))
		{
			gCurrentScene->GetGameObjectManager()->CreateGameObject(mCopyObj);
		}
	}


	Gui::EndWindow();
}

void InspectorWindow::ShowGameObjectData()
{
	for (const auto& component : *mCurrentObj->GetComponentManager()->GetComponents())
	{
		component->ShowDataToInspector();
		Gui::DrawLine();
	}
}

void InspectorWindow::AddComponetButton()
{
	// ボタンのサイズ
	Vec2 buttonSize = Vec2(256, 24);

	// 中心の描画する
	float width = ImGui::GetContentRegionAvail().x;
	ImGui::SetCursorPosX(width / 2 - buttonSize.x / 2);

	Gui::DrawButton("Add Component", buttonSize);
}