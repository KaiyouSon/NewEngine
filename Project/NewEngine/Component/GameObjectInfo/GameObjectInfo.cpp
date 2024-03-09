#include "GameObjectInfo.h"
#include "MainWindow.h"

GameObjectInfo::GameObjectInfo() :
	mChangingName(std::string())
{
}

void GameObjectInfo::ShowGameObjectDataToInspector(GameObject* gameObj)
{
	Gui::DrawCheckBox("Active Flag", &gameObj->isActive);

	Gui::DrawInputText("Object Name", mChangingName);
	if (!ImGui::IsItemActive())
	{
		gameObj->name = mChangingName;
	}

	std::string layerTag = gameObj->layerTag;
	Gui::DrawInputText("Layer Tag", layerTag);

	// ドロップしたときの処理
	if (Gui::DragDropTarget("DragDrop Layer"))
	{
		std::string tag = MainWindow::GetInstance()->GetDragDropLayerTag();
		gameObj->layerTag = tag;
	}

	Gui::DrawColorEdit("Color", gameObj->color);
}

void GameObjectInfo::ShowLightObjectDataToInspector(GameObject* gameObj)
{
	Gui::DrawCheckBox("Active Flag", &gameObj->isActive);

	Gui::DrawInputText("Object Name", mChangingName);
	if (!ImGui::IsItemActive())
	{
		gameObj->name = mChangingName;
	}

	Gui::DrawColorEdit("Color", gameObj->color);
}

void GameObjectInfo::ShowEmptyObjectDataToInspector(GameObject* gameObj)
{
	Gui::DrawCheckBox("Active Flag", &gameObj->isActive);

	Gui::DrawInputText("Object Name", mChangingName);
	if (!ImGui::IsItemActive())
	{
		gameObj->name = mChangingName;
	}
}

void GameObjectInfo::LoadBaseInfoToJson(GameObject* gameObj, const nlohmann::json& componentField)
{
	if (!componentField.contains("base_info"))
	{
		return;
	}

	gameObj->isActive = componentField["base_info"]["is_active"];
	gameObj->layerTag = componentField["base_info"]["layer_tag"];
	gameObj->color.r = componentField["base_info"]["color"][0];
	gameObj->color.g = componentField["base_info"]["color"][1];
	gameObj->color.b = componentField["base_info"]["color"][2];
	gameObj->color.a = componentField["base_info"]["color"][3];
}

nlohmann::json GameObjectInfo::SaveBaseInfoToJson(GameObject* gameObj)
{
	nlohmann::json result;

	result["base_info"]["is_active"] = gameObj->isActive;
	result["base_info"]["layer_tag"] = gameObj->layerTag;
	result["base_info"]["color"] =
	{
		gameObj->color.r,
		gameObj->color.g,
		gameObj->color.b,
		gameObj->color.a
	};

	return result;
}
