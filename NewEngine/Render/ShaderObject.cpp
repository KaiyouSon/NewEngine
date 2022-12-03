#include "ShaderObject.h"
#include <d3dcompiler.h>
#include <string>
#include <cassert>
#pragma comment(lib,"d3dcompiler.lib")

Microsoft::WRL::ComPtr <ID3DBlob> ShaderObject::errorBlob = nullptr;

void ShaderObject::ShowErrorDetails()
{
	// シェーダのエラー内容を表示
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

// 頂点シェーダー
void ShaderObject::CompileVertexShader(
	const std::string& filePath, const std::string& entryPointName)
{
	//// 頂点シェーダに渡すための頂点データを整える
	//inputLayout[0] =
	//{	// xyz座標
	//	"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	//	D3D12_APPEND_ALIGNED_ELEMENT,
	//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	//};
	//inputLayout[1] =
	//{	// xyz座標
	//	"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	//	D3D12_APPEND_ALIGNED_ELEMENT,
	//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	//};
	//inputLayout[2] =
	//{	// uv座標
	//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
	//	D3D12_APPEND_ALIGNED_ELEMENT,
	//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	//};


	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		wFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		entryPointName.c_str(), "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// シェーダのエラー内容を表示
	ShowErrorDetails();

	// 頂点シェーダに渡すための頂点データを整える
	//inputLayout.push_back(
	//	{	// xyz座標
	//		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	//		D3D12_APPEND_ALIGNED_ELEMENT,
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	//	});

	//inputLayout.push_back(
	//	{	// 法線
	//		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	//		D3D12_APPEND_ALIGNED_ELEMENT,
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	//	});

	//inputLayout.push_back(
	//	{	// uv座標
	//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
	//	D3D12_APPEND_ALIGNED_ELEMENT,
	//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	//	});
}

// ジオメトリシェーダー
void ShaderObject::CompileGeometryShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		wFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		entryPointName.c_str(), "gs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// ピクセルシェーダー
void ShaderObject::CompilePixelShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		wFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		entryPointName.c_str(), "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}
