#pragma once
#include "Model.h"
#include "FbxModel.h"
#include <string>
#include <map>

class ModelManager
{
private:
	static std::map<std::string, std::unique_ptr<Model>> modelMap;

public:
	static Model* GetModel(std::string modelTag);
	static Model* LoadObjModel(std::string filePath, std::string modelTag, bool isSmoothing = false);
	static Model* LoadFbxModel(std::string filePath, std::string modelTag);

	static void Destroy();
};