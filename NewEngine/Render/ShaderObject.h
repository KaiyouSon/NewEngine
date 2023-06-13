#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <wrl.h>

class ShaderObject
{
private:
	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_;			// 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob_;			// ジオメトリシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob_;			// ピクセルシェーダオブジェクト
	static Microsoft::WRL::ComPtr <ID3DBlob> sErrorBlob_;	// エラーオブジェクト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_;	// 頂点レイアウト

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
	inline ID3DBlob* GetVSBlob() { return  vsBlob_.Get(); }
	inline ID3DBlob* GetGSBlob() { return  gsBlob_.Get(); }
	inline ID3DBlob* GetPSBlob() { return  psBlob_.Get(); }
	inline static ID3DBlob* GetErrorBlob() { return  sErrorBlob_.Get(); }
	inline const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout() { return inputLayout_; }
	inline void AddInputLayout(const LPCSTR& semanticName, const DXGI_FORMAT& format)
	{
		inputLayout_.push_back(
			{	// xyz座標
				semanticName, 0, format, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			});
	}
};
