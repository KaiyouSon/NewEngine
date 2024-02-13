#include "MainWindow.h"
#include "InspectorWindow.h"
#include "GameObjectManager.h"
#include "ModelManager.h"

void InspectorWindow::DrawDebugGui()
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
	ImGui::SetCursorPosX((width - buttonSize.x) / 2.f);
	ImGui::SetCursorPosX(width / 2 - buttonSize.x / 2);

	Gui::DrawButton("Add Component", buttonSize);
}