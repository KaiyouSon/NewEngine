#pragma once
#include "ShaderObject.h"
#include <map>
#include <string>
#include <memory>

class ShaderObjectManager
{
private:
	static std::map<std::string, std::unique_ptr<ShaderObject>> shaderObjectMap;

public:
	static void Create(const std::string& shaderObjectTag);
	static ShaderObject* GetShaderObject(const std::string& shaderObjectTag);
};

