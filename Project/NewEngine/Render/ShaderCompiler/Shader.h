#pragma once
#include "NewEngineEnum.h"

class Shader
{
private:
	Microsoft::WRL::ComPtr<ID3DBlob> mShaderBlob;
	ShaderType mShaderType;
	HRESULT mResult;

private:
	void ShowErrorMessage();

public:
	Shader() {}
	Shader(const std::string& filePath, const ShaderType shaderType);

public:
	ID3DBlob* GetShaderBlob();
};