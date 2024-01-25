#include "Shader.h"
#include <d3dcompiler.h>

Shader::Shader(const std::string& filePath, const ShaderType shaderType, const std::string& shaderTag) :
	mShaderType(shaderType), mShaderTag(shaderTag)
{
	// 設定してないならコンパイルしない
	if (filePath.empty() == true)
	{
		OutputDebugLog("シェダーファイルパスが無効です");
		return;
	}

	// リリース設定フラグ
	uint32_t flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;

	// デバッグのみ実行
	ProcessAtDebugBuild([&]()
		{
			flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		});

	// stringをwstringに変換
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// シェーダーモデル
	std::string shaderModel = "empty";
	switch (shaderType)
	{
	case ShaderType::Compute:
		shaderModel = "cs_5_0";
		break;
	case ShaderType::Vertex:
		shaderModel = "vs_5_0";
		break;
	case ShaderType::Geometry:
		shaderModel = "gs_5_0";
		break;
	case ShaderType::Pixel:
		shaderModel = "ps_5_0";
		break;
	}

	auto errorBlob = ShaderCompiler::GetErrorBlob();
	// シェーダの読み込みとコンパイル
	mResult = D3DCompileFromFile(
		wFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main",								// エントリーポイント名
		shaderModel.c_str(),				// シェーダーモデル指定
		flags, 0,							// フラグ設定
		&mShaderBlob,
		&errorBlob);

	// コンパイルエラーの詳細を表示
	ShowErrorMessage();
}

void Shader::ShowErrorMessage()
{
	auto errorBlob = ShaderCompiler::GetErrorBlob();
	// シェーダのエラー内容を表示
	if (FAILED(mResult))
	{
		// sErrorBlobからエラー内容をstring型にコピー
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

ID3DBlob* Shader::GetShaderBlob()
{
	return mShaderBlob.Get();
}

std::string Shader::GetShaderTag()
{
	return mShaderTag;
}
