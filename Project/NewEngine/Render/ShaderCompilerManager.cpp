#include "ShaderCompilerManager.h"

std::unordered_map<std::string, std::unique_ptr<ShaderCompiler>> ShaderCompilerManager::sShaderCompilerMap;

void ShaderCompilerManager::Create(const std::string shaderObjectTag)
{
	std::unique_ptr<ShaderCompiler> shaderObject = std::make_unique<ShaderCompiler>();
	sShaderCompilerMap.insert(std::make_pair(shaderObjectTag, std::move(shaderObject)));
}

ShaderCompiler* ShaderCompilerManager::GetShaderCompiler(const std::string shaderObjectTag)
{
	return sShaderCompilerMap[shaderObjectTag].get();
}
