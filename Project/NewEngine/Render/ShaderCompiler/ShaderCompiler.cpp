#include "ShaderCompiler.h"
#include "Util.h"
#include <d3dcompiler.h>
#include <string>
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

Microsoft::WRL::ComPtr <ID3DBlob> ShaderCompiler::sErrorBlob = nullptr;
std::vector<std::string> ShaderCompiler::sShaderTags =
{
	"VS","GS","PS","CS"
};

ShaderCompiler::ShaderCompiler(const ShaderCompilerSetting& shaderCompilerSetting)
	: mResult(HRESULT())
{
	mSetting = shaderCompilerSetting;

	// サイズ分確保
	mShaderBlobs.resize((uint32_t)ShaderType::Size);

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

	// フォルダーパスを設定しているなら
	if (mSetting.folderPath.empty() == false)
	{
		// フォルダ内にある分読み込む
		for (const auto& entry : fs::directory_iterator(mSetting.folderPath))
		{
			auto path = entry.path();

			if (fs::is_regular_file(path))
			{
				// 拡張子
				std::string ext = path.extension().string();

				// ヘッダーなら飛ばす
				if (ext == ".hlsli")
				{
					continue;
				}

				const uint32_t length = static_cast<uint32_t>(path.string().length() - ext.length());

				// 該当のタグのシェーダーをコンパイルする
				for (uint32_t i = 0; i < static_cast<uint32_t>(sShaderTags.size()); i++)
				{
					const uint32_t pos = static_cast<uint32_t>(path.string().find(sShaderTags[i]));
					if (pos != std::string::npos && pos + 2 == length)
					{
						CompileShader(path.string(), (ShaderType)i);
					}
				}
			}
		}
	}
	// フォルダーパスを設定していないなら
	else
	{
		// 各シェーダーをコンパイル
		CompileShader(mSetting.vsFilePath, ShaderType::Vertex);		// VS
		CompileShader(mSetting.gsFilePath, ShaderType::Geometry);	// GS
		CompileShader(mSetting.psFilePath, ShaderType::Pixel);		// PS
		CompileShader(mSetting.csFilePath, ShaderType::Compute);	// CS
	}
}

// シェーダーをコンパイル
void ShaderCompiler::CompileShader(const std::string& filePath, ShaderType shaderType)
{
	// 設定してないならコンパイルしない
	if (filePath.empty() == true)
	{
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

	// シェーダの読み込みとコンパイル
	mResult = D3DCompileFromFile(
		wFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main",								// エントリーポイント名
		shaderModel.c_str(),				// シェーダーモデル指定
		flags,								// フラグ設定
		0,
		&mShaderBlobs[(uint32_t)shaderType], &sErrorBlob);
}

// シェーダーコンパイルエラーの詳細を表示
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

// ゲッター
ID3DBlob* ShaderCompiler::GetErrorBlob()
{
	return  sErrorBlob.Get();
}
ID3DBlob* ShaderCompiler::GetShaderBlob(const ShaderType shaderType)
{
	return mShaderBlobs[(uint32_t)shaderType].Get();
}
const std::vector <D3D12_INPUT_ELEMENT_DESC>& ShaderCompiler::GetInputLayout()
{
	return mInputLayout;
}

