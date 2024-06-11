#include "CPMaterial.h"
#include "ShaderManager.h"

CPMaterial::CPMaterial() {}
CPMaterial::CPMaterial(const std::string& name) :name(name)
{
}

void CPMaterial::LoadToJson(const std::string& path)
{
	mMaterialPath = path;

	// ファイルを開く
	std::ifstream file(path);
	// ファイルが開けない場合はアサーションエラー
	if (file.fail())
	{
		assert(0);
	}

	// JSONをデシリアライズ
	nlohmann::json deserialized;
	file >> deserialized;

	nlohmann::json cpSettingField = deserialized["cp_setting"];

	// シェダーの読み込み
	std::string shaderTag;
	if (cpSettingField.contains("gs"))
	{
		shaderTag = cpSettingField["gs"];
		mCPSetting.cs = ShaderManager::GetShader(ShaderType::Compute, shaderTag);
	}

	// ルートシグネーチャ設定の読み込み
	nlohmann::json rsSettingField = cpSettingField["rs_setting"];
	mRSSetting.maxCbvRootParameter = rsSettingField["max_cbv"];
	mRSSetting.maxSrvDescritorRange = rsSettingField["max_srv"];
	mRSSetting.maxUavDescritorRange = rsSettingField["max_uav"];

	mCPSetting.rootSignatureSetting = mRSSetting;

	file.close();
}
