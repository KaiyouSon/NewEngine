#pragma once
#include "NewEngineSetting.h"
#include "NewEngineEnum.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <wrl.h>

// シェダーコンパイラーのクラス
class ShaderCompiler
{
private:
	static Microsoft::WRL::ComPtr<ID3DBlob> sErrorBlob; // シェーダーコンパイルエラーメッセージの格納
	HRESULT mResult;
	ShaderCompilerSetting mSetting;
	std::vector<Microsoft::WRL::ComPtr<ID3DBlob>> mShaderBlobs;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout; // インプットレイアウトの定義

private:
	// シェーダーコンパイルエラーの詳細を表示
	void ShowErrorDetails();

public:
	// コンストラクタ
	ShaderCompiler() {}
	ShaderCompiler(const ShaderCompilerSetting& shaderCompilerSetting);

	// シェーダーをコンパイル
	void CompileShader(const std::string& filePath, const ShaderType shaderType);

public:
	static ID3DBlob* GetErrorBlob();
	ID3DBlob* GetShaderBlob(const ShaderType shaderType);
	const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout();
};

