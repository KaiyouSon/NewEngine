#include "MaterialManager.h"

namespace fs = std::filesystem;

void MaterialManager::LoadMaterial(const std::string& path)
{
	fs::path fspath = path;
	std::string tag = fspath.filename().string();
	tag = SubString(tag, "Material.json");

	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->LoadToJson(path);

	std::pair pair = std::make_pair(tag, std::move(material));
	mMaterialMap.insert(std::move(pair));
}

Material* MaterialManager::GetMaterial(const std::string& tag)
{
	return mMaterialMap[tag].get();
}
