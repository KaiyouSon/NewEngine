#include "MaterialManager.h"

namespace fs = std::filesystem;

void MaterialManager::LoadMaterial(const std::string& path)
{
	fs::path fspath = path;
	std::string name = fspath.filename().string();
	name = SubString(name, "Material.json");

	std::shared_ptr<Material> material = std::make_shared<Material>(name);
	material->LoadToJson(path);

	std::pair pair = std::make_pair(name, std::move(material));
	mMaterialMap.insert(std::move(pair));
}

Material* MaterialManager::GetMaterial(const std::string& tag)
{
	return mMaterialMap[tag].get();
}
