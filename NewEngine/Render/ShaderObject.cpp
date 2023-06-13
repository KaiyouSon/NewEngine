#include "ShaderObject.h"
#include <d3dcompiler.h>
#include <string>
#include <cassert>
#pragma comment(lib,"d3dcompiler.lib")

Microsoft::WRL::ComPtr <ID3DBlob> ShaderObject::sErrorBlob_ = nullptr;

ShaderObject::ShaderObject() : result_(HRESULT())
{
}

void ShaderObject::ShowErrorDetails()
{
	// シェーダのエラー内容を表示
	if (FAILED(result_))
	{
		// sErrorBlob_からエラー内容をstring型にコピー
		std::string error;
		error.resize(sErrorBlob_->GetBufferSize());
		std::copy_n((char*)sErrorBlob_->GetBufferPointer(),
			sErrorBlob_->GetBufferSize(),
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
	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		wFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		entryPointName.c_str(), "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob_, &sErrorBlob_);

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// ジオメトリシェーダー
void ShaderObject::CompileGeometryShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		wFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		entryPointName.c_str(), "gs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob_, &sErrorBlob_);

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
	result_ = D3DCompileFromFile(
		wFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		entryPointName.c_str(), "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob_, &sErrorBlob_);

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}
