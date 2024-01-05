#pragma once
#include "NewEngineSetting.h"
#include "NewEngineEnum.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <wrl.h>
#include <fstream>
#include <unordered_map>

// シェダーコンパイラーのクラス
class ShaderCompiler
{
private:
	static Microsoft::WRL::ComPtr<ID3DBlob> sErrorBlob; // シェーダーコンパイルエラーメッセージの格納
	static std::vector<std::string> sShaderTags;
	HRESULT mResult;
	ShaderCompilerSetting mSetting;
	std::vector<Microsoft::WRL::ComPtr<ID3DBlob>> mShaderBlobs;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout; // インプットレイアウトの定義

private:
	// シェーダーコンパイルエラーの詳細を表示
	void ShowErrorDetails();

	// 設定ファイルを読み込む
	void LoadIniFile(std::ifstream& file);

public:
	// コンストラクタ
	ShaderCompiler() {}
	ShaderCompiler(const ShaderCompilerSetting& shaderCompilerSetting);
	ShaderCompiler(const std::filesystem::path& filePath);

	// シェーダーをコンパイル
	void CompileShader(const std::string& filePath, const ShaderType shaderType);

public:
	static ID3DBlob* GetErrorBlob();
	ID3DBlob* GetShaderBlob(const ShaderType shaderType);
	const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout();
	ShaderCompilerSetting GetSetting();
};

