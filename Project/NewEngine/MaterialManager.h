#pragma once
#include "StandardLib.h"

class Material;

class MaterialManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<Material>> mMaterialMap;

public:
	void LoadMaterial(const std::string& path);

public:
	Material* GetMaterial(const std::string& tag);
};

