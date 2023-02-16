#pragma once
#include "Model.h"
#include <string>
#include <map>

class ModelManager
{
private:
	static std::map<std::string, std::unique_ptr<Model>> modelMap;

public:
	static Model* GetModel(const std::string& modelTag);
	static Model* LoadObjModel(const std::string& filePath, const std::string& modelTag, const bool& isSmoothing = false);
	static Model* LoadFbxModel(const std::string& filePath, const std::string& modelTag);
};