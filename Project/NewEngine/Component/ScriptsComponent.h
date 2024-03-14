#pragma once
#include"StandardLib.h"
#include"Script.h"

class ScriptsComponent : public Component
{
private:
	std::vector<std::shared_ptr<Script>> mScripts;

public:
	ScriptsComponent();
	ScriptsComponent(GameObject* gameObj);
	void Init();
	void Update() override;
	void CopyComponent(Component* component) override { component; };

public:
	nlohmann::json SaveToJson() override;
	void LoadToJson(const nlohmann::json& componentField) override;
	void ShowDataToInspector() override;
};

