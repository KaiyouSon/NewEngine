#pragma once
#include "NewEngineEnum.h"

class Shader
{
private:
	Microsoft::WRL::ComPtr<ID3DBlob> mShaderBlob;
	ShaderType mShaderType;
	std::string mShaderTag;
	HRESULT mResult;

private:
	void ShowErrorMessage();

public:
	Shader() {}
	Shader(const std::string& filePath, const ShaderType shaderType, const std::string& shaderTag);

public:
	ID3DBlob* GetShaderBlob();
	std::string GetShaderTag();
};