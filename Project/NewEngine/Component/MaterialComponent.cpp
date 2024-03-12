#include "MaterialComponent.h"

MaterialComponent::MaterialComponent() :
	mMaterial(std::make_unique<Material>())
{
	mComponentInfo.type = ComponentType::MaterialComponent;
}

MaterialComponent::MaterialComponent(GameObject* gameObj) :
	Component(gameObj), mMaterial(std::make_unique<Material>())
{
	mComponentInfo.type = ComponentType::MaterialComponent;
}

void MaterialComponent::Update()
{
}

void MaterialComponent::CopyComponent(Component* component)
{
	component;
}


nlohmann::json MaterialComponent::SaveToJson()
{
	nlohmann::json materialComponent;
	materialComponent["material_component"] = mComponentInfo.SaveToJson();
	materialComponent["material_component"]["material_name"] = mMaterial->name;
	return materialComponent;
}

void MaterialComponent::LoadToJson(const nlohmann::json& componentField)
{
	nlohmann::json materialComponentField = componentField["material_component"];

	mComponentInfo.LoadToJson(materialComponentField);

	std::string materialName = materialComponentField["material_name"];
	SetMaterial(materialName);
}

void MaterialComponent::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Material", true))
	{
		std::string label = "empty";
		if (mMaterial)
		{
			label = mMaterial->name;
		}

		const float windowWidth = ImGui::GetContentRegionAvail().x;
		const Vec2 buttonSize = Vec2(windowWidth, 20);
		Gui::DrawButton(label.c_str(), buttonSize);
	}
}

void MaterialComponent::SetMaterial(const std::string& name)
{
	mMaterial->Copy(*gAssetsManager->GetMaterial(name));
}

Material* MaterialComponent::GetMaterial()
{
	return mMaterial.get();
}
