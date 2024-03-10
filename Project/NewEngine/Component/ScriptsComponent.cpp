#include "ScriptsComponent.h"
#include "MainWindow.h"

ScriptsComponent::ScriptsComponent()
{
	mComponentInfo.type = ComponentType::ScriptsComponent;

}

ScriptsComponent::ScriptsComponent(GameObject* gameObj) : Component(gameObj)
{
	mComponentInfo.type = ComponentType::ScriptsComponent;
}

void ScriptsComponent::Update()
{
	for (auto& script : mScripts)
	{
		if (!script)
		{
			continue;
		}

		script->Update();
	}
}

nlohmann::json ScriptsComponent::SaveToJson()
{
	return nlohmann::json();
}

void ScriptsComponent::LoadToJson(const nlohmann::json& componentField)
{
	componentField;
}

void ScriptsComponent::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Scripts", true))
	{
		for (uint32_t i = 0; i < mScripts.size(); i++)
		{
			static std::string tag = "test";

			std::string lable = "Script " + std::to_string(i);
			Gui::DrawInputText(lable.c_str(), tag);

			if (Gui::DragDropTarget("DragDrop Script"))
			{
				std::string scriptTag = MainWindow::GetInstance()->GetDragDropScriptTag();
				mScripts[i] = ScriptManager::GetInstance()->GetScript(scriptTag)->CreateInstence();
				mScripts[i]->SetGameObject(mGameObj);
			}
		}

		// ボタンのサイズ
		Vec2 buttonSize = Vec2(96, 24);

		// 中心の描画する
		float width = ImGui::GetContentRegionAvail().x;
		ImGui::SetCursorPosX(width / 2 - buttonSize.x / 2);

		if (Gui::DrawButton("Add Script", buttonSize))
		{
			mScripts.emplace_back();
		}
	}
}
