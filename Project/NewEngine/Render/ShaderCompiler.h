#pragma once
#include "NewEngineSetting.h"
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
	Microsoft::WRL::ComPtr<ID3DBlob> mCsBlob; // CSコードの格納
	Microsoft::WRL::ComPtr<ID3DBlob> mVsBlob; // VSコードの格納
	Microsoft::WRL::ComPtr<ID3DBlob> mGsBlob; // GSコードの格納
	Microsoft::WRL::ComPtr<ID3DBlob> mPsBlob; // PSコードの格納
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout; // インプットレイアウトの定義

private:
	// シェーダーコンパイルエラーの詳細を表示
	void ShowErrorDetails();

public:
	// コンストラクタ
	ShaderCompiler() {}
	ShaderCompiler(const ShaderCompilerSetting& shaderCompilerSetting);

	// コンピュートシェーダーのコンパイル
	void CompileComputeShader(const std::string& filePath);

	// バーテックスシェーダーのコンパイル
	void CompileVertexShader(const std::string& filePath);

	// ジオメトリシェーダーのコンパイル
	void CompileGeometryShader(const std::string& filePath);

	// ピクセルシェーダーのコンパイル
	void CompilePixelShader(const std::string& filePath);

public:
	ID3DBlob* GetCSBlob();
	ID3DBlob* GetVSBlob();
	ID3DBlob* GetGSBlob();
	ID3DBlob* GetPSBlob();
	static ID3DBlob* GetErrorBlob();
	const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout();
};

