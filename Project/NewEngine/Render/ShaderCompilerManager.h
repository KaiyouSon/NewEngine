#pragma once
#include "ShaderCompiler.h"
#include <unordered_map>
#include <memory>

class ShaderCompilerManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<ShaderCompiler>> sShaderCompilerMap;

public:
	static void Create(const std::string shaderObjectTag);
	static ShaderCompiler* GetShaderCompiler(const std::string shaderObjectTag);
};

