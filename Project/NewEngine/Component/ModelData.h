#pragma once
#include "StandardLib.h"

class Model;

class ModelData : public Component
{
private:
	Model* mModel;

public:
	ModelData();
	ModelData(GameObject* gameObj);
	void Update() override;

public:
	Model* GetModel();
	void SetModel(const std::string& tag);

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;

};