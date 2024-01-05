#include "ShaderCompilerManager.h"

std::unordered_map<std::string, std::shared_ptr<ShaderCompiler>> ShaderCompilerManager::sShaderCompilerMap;

void ShaderCompilerManager::Create(const ShaderCompilerSetting& setting, const std::string tag)
{
	std::shared_ptr<ShaderCompiler> shaderCompiler = std::make_shared<ShaderCompiler>(setting);

	// マップに格納
	sShaderCompilerMap.insert(std::make_pair(tag, shaderCompiler));
}

void ShaderCompilerManager::Create(const std::filesystem::path& folderPath, const std::string tag)
{
	std::shared_ptr<ShaderCompiler> shaderCompiler = std::make_shared<ShaderCompiler>(folderPath);

	// マップに格納
	sShaderCompilerMap.insert(std::make_pair(tag, shaderCompiler));
}

void ShaderCompilerManager::ReCompiler(const std::filesystem::path& folderPath, const std::string tag)
{
	std::shared_ptr<ShaderCompiler> shaderCompiler = std::make_shared<ShaderCompiler>(folderPath);

	// 上書き
	sShaderCompilerMap[tag] = shaderCompiler;
}

ShaderCompiler* ShaderCompilerManager::GetShaderCompiler(const std::string tag)
{
	return sShaderCompilerMap[tag].get();
}

std::unordered_map<std::string, std::shared_ptr<ShaderCompiler>>* ShaderCompilerManager::GetShaderCompilerMap()
{
	return &sShaderCompilerMap;
}
