#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <wrl.h>

class ShaderObject
{
private:
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;			// 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob;			// ジオメトリシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;			// ピクセルシェーダオブジェクト
	static Microsoft::WRL::ComPtr <ID3DBlob> errorBlob;	// エラーオブジェクト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;	// 頂点レイアウト

private:
	// シェーダのエラー内容を表示する関数
	void ShowErrorDetails();

public:
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
	inline ID3DBlob* GetVSBlob() { return  vsBlob.Get(); }
	inline ID3DBlob* GetGSBlob() { return  gsBlob.Get(); }
	inline ID3DBlob* GetPSBlob() { return  psBlob.Get(); }
	inline const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout() { return inputLayout; }
	inline void AddInputLayout(const LPCSTR& semanticName, const DXGI_FORMAT& format)
	{
		inputLayout.push_back(
			{	// xyz座標
				semanticName, 0, format, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			});
	}
};
