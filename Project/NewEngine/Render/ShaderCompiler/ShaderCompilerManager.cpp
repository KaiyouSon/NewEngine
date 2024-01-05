#include "ShaderCompilerManager.h"

std::unordered_map<std::string, std::unique_ptr<ShaderCompiler>> ShaderCompilerManager::sShaderCompilerMap;

void ShaderCompilerManager::Create(const ShaderCompilerSetting& setting, const std::string tag)
{
	std::unique_ptr<ShaderCompiler> shaderCompiler = std::make_unique<ShaderCompiler>(setting);

	// マップに格納
	sShaderCompilerMap.insert(std::make_pair(tag, std::move(shaderCompiler)));
}

void ShaderCompilerManager::Create(const std::filesystem::path& folderPath, const std::string tag)
{
	std::unique_ptr<ShaderCompiler> shaderCompiler = std::make_unique<ShaderCompiler>();
	shaderCompiler->Create(folderPath);

	// マップに格納
	sShaderCompilerMap.insert(std::make_pair(tag, std::move(shaderCompiler)));
}

void ShaderCompilerManager::ReCompiler(const std::filesystem::path& folderPath, const std::string tag)
{
	// 上書き
	sShaderCompilerMap[tag]->Create(folderPath);
}

ShaderCompiler* ShaderCompilerManager::GetShaderCompiler(const std::string tag)
{
	return sShaderCompilerMap[tag].get();
}

std::unordered_map<std::string, std::unique_ptr<ShaderCompiler>>* ShaderCompilerManager::GetShaderCompilerMap()
{
	return &sShaderCompilerMap;
}
