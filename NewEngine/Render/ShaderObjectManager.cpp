#include "ShaderObjectManager.h"

std::unordered_map<std::string, std::unique_ptr<ShaderObject>> ShaderObjectManager::sShaderObjectMap_;

void ShaderObjectManager::Create(const std::string shaderObjectTag)
{
	std::unique_ptr<ShaderObject> shaderObject = std::make_unique<ShaderObject>();
	sShaderObjectMap_.insert(std::make_pair(shaderObjectTag, std::move(shaderObject)));
}

ShaderObject* ShaderObjectManager::GetShaderObject(const std::string shaderObjectTag)
{
	return sShaderObjectMap_[shaderObjectTag].get();
}
