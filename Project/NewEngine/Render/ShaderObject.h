#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <wrl.h>

class ShaderObject
{
private:
	static Microsoft::WRL::ComPtr <ID3DBlob> sErrorBlob;	// エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> mVsBlob;			// 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> mGsBlob;			// ジオメトリシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> mPsBlob;			// ピクセルシェーダオブジェクト
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;	// 頂点レイアウト
	HRESULT mResult;

private:
	// シェーダのエラー内容を表示する関数
	void ShowErrorDetails();

public:
	ShaderObject();

	// 頂点シェーダーのコンパイル
	void CompileVertexShader(
		const std::string& filePath,
		const std::string& entryPointName);

	// ジオメトリシェーダーのコンパイル
	void CompileGeometryShader(
		const std::string& filePath,
		const std::string& entryPointName);

	// ピクセルシェーダーのコンパイル
	void CompilePixelShader(
		const std::string& filePath,
		const std::string& entryPointName);

public:
	ID3DBlob* GetVSBlob();
	ID3DBlob* GetGSBlob();
	ID3DBlob* GetPSBlob();
	static ID3DBlob* GetErrorBlob();
	const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout();
	void AddInputLayout(const LPCSTR& semanticName, const DXGI_FORMAT format, const uint32_t index = 0);
};
