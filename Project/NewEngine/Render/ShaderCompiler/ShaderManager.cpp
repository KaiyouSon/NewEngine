#include "ShaderManager.h"

namespace fs = std::filesystem;

ShaderManager::ShaderManager()
{
	// シェーダー識別子
	mShaderTags[(uint32_t)ShaderType::Vertex] = "VS";
	mShaderTags[(uint32_t)ShaderType::Geometry] = "GS";
	mShaderTags[(uint32_t)ShaderType::Pixel] = "PS";
	mShaderTags[(uint32_t)ShaderType::Compute] = "CS";
}

void ShaderManager::Compile(const std::filesystem::path& folderPath, const ShaderType shaderType, const std::string& tag)
{
	std::string filePath = folderPath.string() + "/" + folderPath.filename().string();
	switch (shaderType)
	{
	case ShaderType::Vertex:
		// インプットレイアウトの読み込み
		GetInstance()->LoadInputLayout(filePath + ".ilt", tag);
		filePath += "VS.hlsl";
		break;
	case ShaderType::Geometry:
		filePath += "GS.hlsl";
		break;
	case ShaderType::Pixel:
		filePath += "PS.hlsl";
		break;
	case ShaderType::Compute:
		filePath += "CS.hlsl";
		break;
	}

	// シェーダーファイルのコンパイル
	std::unique_ptr<Shader> shader =
		std::make_unique<Shader>(filePath, shaderType);

	// ペアを作成してマップに格納
	std::pair pair = std::make_pair(tag, std::move(shader));
	GetInstance()->mShaderMaps[(uint32_t)shaderType].insert(std::move(pair));
}

void ShaderManager::CompileAllType(const std::filesystem::path& folderPath, const std::string& tag)
{
	// フォルダ内にある分読み込む
	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		auto path = entry.path();

		// 拡張子
		std::string ext = path.extension().string();

		// インプットレイアウトの読み込み
		if (ext == ".ilt")
		{
			GetInstance()->LoadInputLayout(path.string(), tag);
		}

		// hlslファイルじゃなければ飛ばす
		if (ext != ".hlsl")
		{
			continue;
		}

		const uint32_t length = (uint32_t)(path.string().length() - ext.length());

		// 該当の識別子のシェーダーをコンパイルする
		for (uint32_t i = 0; i < (uint32_t)GetInstance()->mShaderTags.size(); i++)
		{
			const uint32_t pos = (uint32_t)path.string().find(GetInstance()->mShaderTags[i]);
			if (pos != std::string::npos && pos + 2 == length)
			{
				// シェーダーファイルのコンパイル
				ShaderType shaderType = (ShaderType)i;
				std::unique_ptr<Shader> shader =
					std::make_unique<Shader>(path.string(), shaderType);

				// ペアを作成してマップに格納
				std::pair pair = std::make_pair(tag, std::move(shader));
				GetInstance()->mShaderMaps[(uint32_t)shaderType].insert(std::move(pair));
			}
		}
	}
}

void ShaderManager::LoadInputLayout(const std::string& filePath, const std::string& tag)
{
	std::unique_ptr<InputLayout> inputLayout = std::make_unique<InputLayout>(filePath);

	std::pair pair = std::make_pair(tag, std::move(std::make_unique<InputLayout>(filePath)));
	mInputLayout.insert(std::move(pair));
}

Shader* ShaderManager::GetShader(const ShaderType shaderType, const std::string& tag)
{
	auto& map = GetInstance()->mShaderMaps[(uint32_t)shaderType];
	auto it = map.find(tag);
	if (it == map.end())
	{
		return nullptr;
	}

	return map[tag].get();
}

InputLayout* ShaderManager::GetInputLayout(const std::string& tag)
{
	auto& map = GetInstance()->mInputLayout;
	auto it = map.find(tag);
	if (it == map.end())
	{
		return nullptr;
	}

	return map[tag].get();
}

std::unordered_map<std::string, std::unique_ptr<Shader>>* ShaderManager::GetShaderMap(const ShaderType shaderType)
{
	return &GetInstance()->mShaderMaps[(uint32_t)shaderType];
}
