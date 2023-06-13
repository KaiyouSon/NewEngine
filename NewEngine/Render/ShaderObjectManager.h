#pragma once
#include "ShaderObject.h"
#include <unordered_map>
#include <memory>

class ShaderObjectManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<ShaderObject>> sShaderObjectMap_;

public:
	static void Create(const std::string shaderObjectTag);
	static ShaderObject* GetShaderObject(const std::string shaderObjectTag);
};

