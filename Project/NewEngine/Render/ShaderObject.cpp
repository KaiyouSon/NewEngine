#include "ShaderObject.h"
#include "Util.h"
#include <d3dcompiler.h>
#include <string>
#include <cassert>
#pragma comment(lib,"d3dcompiler.lib")

Microsoft::WRL::ComPtr <ID3DBlob> ShaderObject::sErrorBlob = nullptr;

ShaderObject::ShaderObject() : mResult(HRESULT())
{
}

void ShaderObject::ShowErrorDetails()
{
	// シェーダのエラー内容を表示
	if (FAILED(mResult))
	{
		// sErrorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(sErrorBlob->GetBufferSize());
		std::copy_n((char*)sErrorBlob->GetBufferPointer(),
			sErrorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

// コンピュートシェーダー
void ShaderObject::CompileComputeShader(const std::string& filePath, const std::string& entryPointName)
{
	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// デバッグのみ実行
	ProcessAtDebugBulid([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				entryPointName.c_str(), "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
				0,
				&mCsBlob, &sErrorBlob);
		});

	// リリースのみ実行
	ProcessAtReleaseBulid([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				entryPointName.c_str(), "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
				D3DCOMPILE_OPTIMIZATION_LEVEL3, // デバッグ用設定
				0,
				&mCsBlob, &sErrorBlob);
		});

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// 頂点シェーダー
void ShaderObject::CompileVertexShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// デバッグのみ実行
	ProcessAtDebugBulid([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				entryPointName.c_str(), "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
				0,
				&mVsBlob, &sErrorBlob);
		});

	// リリースのみ実行
	ProcessAtReleaseBulid([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				entryPointName.c_str(), "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
				D3DCOMPILE_OPTIMIZATION_LEVEL3, // デバッグ用設定
				0,
				&mVsBlob, &sErrorBlob);
		});

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// ジオメトリシェーダー
void ShaderObject::CompileGeometryShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// デバッグのみ実行
	ProcessAtDebugBulid([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				entryPointName.c_str(), "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
				0,
				&mGsBlob, &sErrorBlob);
		});

	// リリースのみ実行
	ProcessAtReleaseBulid([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				entryPointName.c_str(), "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
				D3DCOMPILE_OPTIMIZATION_LEVEL3, // デバッグ用設定
				0,
				&mGsBlob, &sErrorBlob);
		});

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// ピクセルシェーダー
void ShaderObject::CompilePixelShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// デバッグのみ実行
	ProcessAtDebugBulid([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				entryPointName.c_str(), "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
				0,
				&mPsBlob, &sErrorBlob);
		});


	// リリースのみ実行
	ProcessAtReleaseBulid([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				entryPointName.c_str(), "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
				D3DCOMPILE_OPTIMIZATION_LEVEL3, // デバッグ用設定
				0,
				&mPsBlob, &sErrorBlob);
		});

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// インプットレイアウトの追加
void ShaderObject::AddInputLayout(const LPCSTR& semanticName, const DXGI_FORMAT format, const uint32_t index)
{
	mInputLayout.push_back(
		{	// xyz座標
			semanticName, index, format, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		});
}

// ゲッター
ID3DBlob* ShaderObject::GetErrorBlob()
{
	return  sErrorBlob.Get();
}
ID3DBlob* ShaderObject::GetCSBlob()
{
	return mCsBlob.Get();
}
ID3DBlob* ShaderObject::GetVSBlob()
{
	return mVsBlob.Get();
}
ID3DBlob* ShaderObject::GetGSBlob()
{
	return mGsBlob.Get();
}
ID3DBlob* ShaderObject::GetPSBlob()
{
	return mPsBlob.Get();
}
const std::vector <D3D12_INPUT_ELEMENT_DESC>& ShaderObject::GetInputLayout()
{
	return mInputLayout;
}

