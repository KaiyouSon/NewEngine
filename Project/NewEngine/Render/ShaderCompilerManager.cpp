#include "ShaderCompilerManager.h"

std::unordered_map<std::string, std::unique_ptr<ShaderCompiler>> ShaderCompilerManager::sShaderCompilerMap;

void ShaderCompilerManager::Create(const std::string shaderObjectTag)
{
	std::unique_ptr<ShaderCompiler> shaderObject = std::make_unique<ShaderCompiler>();
	sShaderCompilerMap.insert(std::make_pair(shaderObjectTag, std::move(shaderObject)));
}

void ShaderCompilerManager::Create(const ShaderCompilerSetting& setting, const std::string tag)
{
	std::unique_ptr<ShaderCompiler> shaderCompiler =
		std::make_unique<ShaderCompiler>(setting);

	// マップの格納
	sShaderCompilerMap.insert(std::make_pair(tag, std::move(shaderCompiler)));
}

ShaderCompiler* ShaderCompilerManager::GetShaderCompiler(const std::string tag)
{
	return sShaderCompilerMap[tag].get();
}
