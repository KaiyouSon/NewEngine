#include "Material.h"
using namespace std;

Material::Material() :
	ambient(0.3f, 0.3f, 0.3f), diffuse(0.0f, 0.0f, 0.0f),
	specular(0.0f, 0.0f, 0.0f), alpha(1.0f), name("error")
{}

void MaterialManager::AddMaterial(std::unique_ptr<Material>&& material, const uint32_t& index)
{
	list.push_back(move(material));
	list.back()->SetIndex(index);
}

Material* MaterialManager::GetMaterial(const uint32_t& index)
{
	for (const auto& temp : list)
	{
		if (temp->GetIndex() == index)
		{
			return temp.get();
		}
	}
	return nullptr;
}

void MaterialManager::DestroyMaterial(const uint32_t& index)
{
	for (const auto& temp : list)
	{
		if (temp->GetIndex() == index)
		{
			list.remove(temp);
			break;
		}
	}
}

unique_ptr<MaterialManager> materialList = move(make_unique<MaterialManager>());
