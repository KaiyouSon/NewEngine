#pragma once
#include "ShaderCompiler.h"
#include <unordered_map>
#include <memory>

// シェーダーコンパイラーを管理するクラス
class ShaderCompilerManager
{
private:
	// シェーダーコンパイラーのマップ
	static std::unordered_map<std::string, std::unique_ptr<ShaderCompiler>> sShaderCompilerMap;

public:
	// シェーダーコンパイラーを作成する関数
	static void Create(const ShaderCompilerSetting& setting, const std::string tag);
	static void Create(const std::filesystem::path& folderPath, const std::string tag);

	// シェーダーコンパイラーを取得する関数
	static ShaderCompiler* GetShaderCompiler(const std::string tag);
};

