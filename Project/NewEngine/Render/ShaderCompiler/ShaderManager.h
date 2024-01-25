#pragma once
#include "StandardLib.h"
#include "Shader.h"
#include "InputLayout.h"
#include "Singleton.h"

// 前方宣言
template<typename T> class Singleton;

// シェーダーを管理するクラス
class ShaderManager : public Singleton<ShaderManager>
{
private:
	// シェーダー関連
	std::array<std::unordered_map<std::string, std::unique_ptr<Shader>>, (uint32_t)ShaderType::Count> mShaderMaps;
	std::array<std::string, (uint32_t)ShaderType::Count> mShaderTags;

	// インプットレイアウト関連(VSで使うため)
	std::unordered_map<std::string, std::unique_ptr<InputLayout>> mInputLayout;

private:
	void LoadInputLayout(const std::string& filePath, const std::string& tag);

public:
	// コンパイル関連
	static void Compile(const std::string& filePath, const ShaderType shaderType, const std::string& tag);
	static void CompileAllType(const std::filesystem::path& folderPath, const std::string& tag = std::string());

public:
	// 取得関連
	static Shader* GetShader(const ShaderType shaderType, const std::string& tag);
	static InputLayout* GetInputLayout(const std::string& tag);
	static std::unordered_map<std::string, std::unique_ptr<Shader>>* GetShaderMap(const ShaderType shaderType);

public:
	// シングルトーン関連
	friend Singleton<ShaderManager>;
	ShaderManager();
};

