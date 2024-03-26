#pragma once
#include "StandardLib.h"

class GameObjectInfo
{
protected:
	std::string mChangingName;

private:
	void ShowActiveGui(GameObject* gameObj);
	void ShowNameGui(GameObject* gameObj);
	void ShowLayerTagGui(GameObject* gameObj);

protected:
	void ShowGameObjectDataToInspector(GameObject* gameObj);
	void ShowLightObjectDataToInspector(GameObject* gameObj);
	void ShowEmptyObjectDataToInspector(GameObject* gameObj);

protected:
	void LoadBaseInfoToJson(GameObject* gameObj, const nlohmann::json& componentField);
	nlohmann::json SaveBaseInfoToJson(GameObject* gameObj);

public:
	GameObjectInfo();
};

