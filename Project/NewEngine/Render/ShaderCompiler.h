#pragma once
#include "NewEngineSetting.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <wrl.h>

class ShaderCompiler
{
private:
	static Microsoft::WRL::ComPtr <ID3DBlob> sErrorBlob;// 繧ｨ繝ｩ繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝・
	HRESULT mResult;
	ShaderCompilerSetting mSetting;
	Microsoft::WRL::ComPtr<ID3DBlob> mCsBlob;			// 鬆らせ繧ｷ繧ｧ繝ｼ繝繧ｪ繝悶ず繧ｧ繧ｯ繝・
	Microsoft::WRL::ComPtr<ID3DBlob> mVsBlob;			// 鬆らせ繧ｷ繧ｧ繝ｼ繝繧ｪ繝悶ず繧ｧ繧ｯ繝・
	Microsoft::WRL::ComPtr<ID3DBlob> mGsBlob;			// 繧ｸ繧ｪ繝｡繝医Μ繧ｷ繧ｧ繝ｼ繝繧ｪ繝悶ず繧ｧ繧ｯ繝・
	Microsoft::WRL::ComPtr<ID3DBlob> mPsBlob;			// 繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繧ｪ繝悶ず繧ｧ繧ｯ繝・
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;	// 鬆らせ繝ｬ繧､繧｢繧ｦ繝・

private:
	// 繧ｷ繧ｧ繝ｼ繝縺ｮ繧ｨ繝ｩ繝ｼ蜀・ｮｹ繧定｡ｨ遉ｺ縺吶ｋ髢｢謨ｰ
	void ShowErrorDetails();

public:
	// コンストラクタ
	ShaderCompiler() {}
	ShaderCompiler(const ShaderCompilerSetting& shaderCompilerSetting);

	// 繧ｳ繝ｳ繝斐Η繝ｼ繝医す繧ｧ繝ｼ繝繝ｼ縺ｮ繧ｳ繝ｳ繝代う繝ｫ
	void CompileComputeShader(const std::string& filePath);

	// 鬆らせ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ繧ｳ繝ｳ繝代う繝ｫ
	void CompileVertexShader(const std::string& filePath);

	// 繧ｸ繧ｪ繝｡繝医Μ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ繧ｳ繝ｳ繝代う繝ｫ
	void CompileGeometryShader(const std::string& filePath);

	// 繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ繧ｳ繝ｳ繝代う繝ｫ
	void CompilePixelShader(const std::string& filePath);

public:
	ID3DBlob* GetCSBlob();
	ID3DBlob* GetVSBlob();
	ID3DBlob* GetGSBlob();
	ID3DBlob* GetPSBlob();
	static ID3DBlob* GetErrorBlob();
	const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout();
};

