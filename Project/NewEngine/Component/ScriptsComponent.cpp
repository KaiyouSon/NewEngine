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

void ScriptsComponent::Init()
{
	for (auto& script : mScripts)
	{
		if (!script)
		{
			continue;
		}

		script->Init();
	}
}

void ScriptsComponent::Update()
{
	if (!gCurrentScene->GetisActive())
	{
		return;
	}

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
	nlohmann::json scriptsComponent;
	scriptsComponent["scripts_component"] = mComponentInfo.SaveToJson();

	nlohmann::json scriptsData = nlohmann::json::array();
	for (auto& script : mScripts)
	{
		nlohmann::json scriptData;
		if (script)
		{
			scriptData["script_tag"] = script->GetTag();
		}
		else
		{
			scriptData["script_tag"] = std::string();
		}

		scriptsData.push_back(scriptData);
	}

	scriptsComponent["scripts_component"]["scripts"] = scriptsData;
	return scriptsComponent;
}

void ScriptsComponent::LoadToJson(const nlohmann::json& componentField)
{
	nlohmann::json scriptsComponent = componentField["scripts_component"];
	mComponentInfo.LoadToJson(scriptsComponent);

	for (auto& scriptData : scriptsComponent["scripts"])
	{
		mScripts.emplace_back();

		std::string scriptTag = scriptData["script_tag"];
		if (!scriptTag.empty())
		{
			mScripts.back() = ScriptManager::GetInstance()->GetScript(scriptTag)->CreateInstence();
			mScripts.back()->SetGameObject(mGameObj);
		}
	}
}

void ScriptsComponent::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Scripts", true))
	{
		for (uint32_t i = 0; i < mScripts.size(); i++)
		{
			const std::string tag = mScripts[i] == nullptr ? "empty " + std::to_string(i) : mScripts[i]->GetTag();
			const float windowWidth = ImGui::GetContentRegionAvail().x;
			const Vec2 buttonSize = Vec2(windowWidth, 20);
			Gui::DrawButton(tag.c_str(), buttonSize);

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
