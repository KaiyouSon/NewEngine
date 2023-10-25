#include "ShaderCompiler.h"
#include "Util.h"
#include <d3dcompiler.h>
#include <string>
#include <cassert>

Microsoft::WRL::ComPtr <ID3DBlob> ShaderCompiler::sErrorBlob = nullptr;

ShaderCompiler::ShaderCompiler(const ShaderCompilerSetting& shaderCompilerSetting)
	: mResult(HRESULT())
{
	mSetting = shaderCompilerSetting;

	// インプットレイアウトの設定
	for (uint32_t i = 0; i < mSetting.mInputLayoutSettings.size(); i++)
	{
		mInputLayout.push_back(
			{
				mSetting.mInputLayoutSettings[i].semanticName.c_str(),
				mSetting.mInputLayoutSettings[i].index,
				mSetting.mInputLayoutSettings[i].format,
				0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				0
			});
	}

	// シェーダーコンパイル
	CompileComputeShader(mSetting.csFilePath);
	CompileVertexShader(mSetting.vsFilePath);
	CompileGeometryShader(mSetting.gsFilePath);
	CompilePixelShader(mSetting.psFilePath);
}

void ShaderCompiler::ShowErrorDetails()
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
void ShaderCompiler::CompileComputeShader(const std::string& filePath)
{
	// 設定してないならコンパイルしない
	if (filePath.empty() == true)
	{
		return;
	}

	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// デバッグのみ実行
	ProcessAtDebugBuild([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				"main",								// エントリーポイント名
				"cs_5_0",							// シェーダーモデル指定
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
				0,
				&mCsBlob, &sErrorBlob);
		});

	// リリースのみ実行
	ProcessAtReleaseBuild([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				"main",								// エントリーポイント名
				"cs_5_0",							// シェーダーモデル指定
				D3DCOMPILE_OPTIMIZATION_LEVEL3,		// リリース用設定
				0,
				&mCsBlob, &sErrorBlob);
		});

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// 頂点シェーダー
void ShaderCompiler::CompileVertexShader(const std::string& filePath)
{
	// 設定してないならコンパイルしない
	if (filePath.empty() == true)
	{
		return;
	}

	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// デバッグのみ実行
	ProcessAtDebugBuild([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				"main",								// エントリーポイント名
				"vs_5_0",							// シェーダーモデル指定
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
				0,
				&mVsBlob, &sErrorBlob);
		});

	// リリースのみ実行
	ProcessAtReleaseBuild([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				"main",								// エントリーポイント名
				"vs_5_0",							// シェーダーモデル指定
				D3DCOMPILE_OPTIMIZATION_LEVEL3,		// リリース用設定
				0,
				&mVsBlob, &sErrorBlob);
		});

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// ジオメトリシェーダー
void ShaderCompiler::CompileGeometryShader(const std::string& filePath)
{
	// 設定してないならコンパイルしない
	if (filePath.empty() == true)
	{
		return;
	}

	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// デバッグのみ実行
	ProcessAtDebugBuild([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				"main",								// エントリーポイント名
				"gs_5_0",							// シェーダーモデル指定
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
				0,
				&mGsBlob, &sErrorBlob);
		});

	// リリースのみ実行
	ProcessAtReleaseBuild([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				"main",								// エントリーポイント名
				"gs_5_0",							// シェーダーモデル指定
				D3DCOMPILE_OPTIMIZATION_LEVEL3,		// リリース用設定
				0,
				&mGsBlob, &sErrorBlob);
		});

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// ピクセルシェーダー
void ShaderCompiler::CompilePixelShader(const std::string& filePath)
{
	// 設定してないならコンパイルしない
	if (filePath.empty() == true)
	{
		return;
	}

	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// デバッグのみ実行
	ProcessAtDebugBuild([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				"main",								// エントリーポイント名
				"ps_5_0",							// シェーダーモデル指定
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
				0,
				&mPsBlob, &sErrorBlob);
		});


	// リリースのみ実行
	ProcessAtReleaseBuild([&]()
		{
			// シェーダの読み込みとコンパイル
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
				"main",								// エントリーポイント名
				"ps_5_0",							// シェーダーモデル指定
				D3DCOMPILE_OPTIMIZATION_LEVEL3,		// リリース用設定
				0,
				&mPsBlob, &sErrorBlob);
		});

	// シェーダのエラー内容を表示
	ShowErrorDetails();
}

// ゲッター
ID3DBlob* ShaderCompiler::GetErrorBlob()
{
	return  sErrorBlob.Get();
}
ID3DBlob* ShaderCompiler::GetCSBlob()
{
	return mCsBlob.Get();
}
ID3DBlob* ShaderCompiler::GetVSBlob()
{
	return mVsBlob.Get();
}
ID3DBlob* ShaderCompiler::GetGSBlob()
{
	return mGsBlob.Get();
}
ID3DBlob* ShaderCompiler::GetPSBlob()
{
	return mPsBlob.Get();
}
const std::vector <D3D12_INPUT_ELEMENT_DESC>& ShaderCompiler::GetInputLayout()
{
	return mInputLayout;
}

