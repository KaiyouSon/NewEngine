#include "ShaderObjectManager.h"

std::map<std::string, std::unique_ptr<ShaderObject>> ShaderObjectManager::shaderObjectMap;

void ShaderObjectManager::Create(const std::string& shaderObjectTag)
{
	std::unique_ptr<ShaderObject> shaderObject = std::make_unique<ShaderObject>();
	shaderObjectMap.insert(std::make_pair(shaderObjectTag, std::move(shaderObject)));
}

ShaderObject* ShaderObjectManager::GetShaderObject(const std::string& shaderObjectTag)
{
	return shaderObjectMap[shaderObjectTag].get();
}
