#include "ModelManager.h"

std::map<std::string, std::unique_ptr<Model>> ModelManager::modelMap;

Model* ModelManager::GetModel(const std::string& modelTag)
{
	return modelMap[modelTag].get();
}

Model* ModelManager::LoadModel(const std::string& filePath, const std::string& modelTag, const bool& isSmoothing)
{
	std::unique_ptr<Model> tex;
	tex.reset(new Model(filePath, isSmoothing));
	modelMap.insert(std::make_pair(modelTag, std::move(tex)));

	return modelMap[modelTag].get();
}
