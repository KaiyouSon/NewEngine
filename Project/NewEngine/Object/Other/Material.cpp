#include "Material.h"
#include "RenderBase.h"
#include "ShaderManager.h"
#include "TextureComponent.h"

Material::Material()
{

}
Material::Material(const std::string& name) :name(name)
{

}

void Material::Init()
{
	// CBの生成
	for (const auto& buffer : constantBuffers)
	{
		if (!buffer)
		{
			continue;
		}

		buffer->Create();
	}
}

void Material::Create()
{
	// マテリアルのJsonファイル生成
	CreateJsonFile();

	// パイプライン生成
	mGraphicsPipeline = std::make_unique<GraphicsPipeline>();
	mGraphicsPipeline->Create(*this);
}

void Material::DrawCommands(TextureComponent* textureData, const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// CBVの描画コマンド
	const uint32_t cbvStartIndex = 0;
	const uint32_t cbvEndIndex = mRSSetting.maxCbvRootParameter;
	for (uint32_t i = cbvStartIndex; i < cbvEndIndex; i++)
	{
		if (!constantBuffers[i])
		{
			continue;
		}

		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}

	// SRVの描画コマンド
	const uint32_t srvStartIndex = cbvEndIndex;
	const uint32_t srvEndIndex = mRSSetting.maxCbvRootParameter + mRSSetting.maxSrvDescritorRange;
	for (uint32_t i = srvStartIndex; i < srvEndIndex; i++)
	{
		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
			i, textureData->GetTexture()->GetBufferResource()->srvHandle.gpu);
	}
}

void Material::DrawCommands(std::vector<ITexture*> textures, const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// CBVの描画コマンド
	const uint32_t cbvStartIndex = 0;
	const uint32_t cbvEndIndex = mRSSetting.maxCbvRootParameter;
	for (uint32_t i = cbvStartIndex; i < cbvEndIndex; i++)
	{
		if (!constantBuffers[i])
		{
			continue;
		}

		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}

	// SRVの描画コマンド
	const uint32_t srvStartIndex = cbvEndIndex;
	const uint32_t srvEndIndex = mRSSetting.maxCbvRootParameter + mRSSetting.maxSrvDescritorRange;
	for (uint32_t i = srvStartIndex, texIndex = 0; i < srvEndIndex; i++, texIndex++)
	{
		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
			i, textures[texIndex]->GetBufferResource()->srvHandle.gpu);
	}
}

void Material::Copy(const Material& material)
{
	name = material.name;
	mRSSetting = material.mRSSetting;
	mGPSetting = material.mGPSetting;

	constantBuffers.resize(mRSSetting.maxCbvRootParameter);
	structuredBuffers.resize(mRSSetting.maxUavDescritorRange);

	// パイプライン生成
	mGraphicsPipeline = std::make_unique<GraphicsPipeline>();
	mGraphicsPipeline->Create(*this);

}

void Material::CreateJsonFile()
{
	//// JSONオブジェクトを作成
	//nlohmann::json data;
	//data["gp_setting"] = "12";
	//data["rs_setting"] = "34";

	//// JSONをファイルに書き込む
	//std::string path = EngineDataDirectory + "Scene/" + sceneName + "Scene.json";

	//std::ofstream outputFile(path);
	//outputFile << std::setw(4) << data << std::endl;
	//outputFile.close();
}

void Material::LoadToJson(const std::string& path)
{
	//std::string path = EngineDataDirectory + "Scene/" + sceneName + "Scene.json";

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

	// グラフィックスパイプライン設定の読み込み
	nlohmann::json gpSettingField = deserialized["gp_setting"];
	mGPSetting.cullMode = (CullMode)gpSettingField["cull_mode"];
	mGPSetting.fillMode = (FillMode)gpSettingField["fill_mode"];
	mGPSetting.topologyType = (TopologyType)gpSettingField["topology_type"];
	mGPSetting.renderTargetBlendMode = (GraphicsPipelineSetting::RenderTargetBlendMode)gpSettingField["rt_blend_mode"];
	mGPSetting.renderTargetBlendMask = (GraphicsPipelineSetting::RenderTargetBlendMask)gpSettingField["rt_blend_mask"];
	mGPSetting.rtvNum = gpSettingField["rtv_num"];

	std::string shaderTag;
	if (gpSettingField.contains("vs"))
	{
		shaderTag = gpSettingField["vs"];
		mGPSetting.vs = ShaderManager::GetShader(ShaderType::Vertex, shaderTag);
	}
	if (gpSettingField.contains("gs"))
	{
		shaderTag = gpSettingField["gs"];
		mGPSetting.gs = ShaderManager::GetShader(ShaderType::Geometry, shaderTag);
	}
	if (gpSettingField.contains("ps"))
	{
		shaderTag = gpSettingField["ps"];
		mGPSetting.ps = ShaderManager::GetShader(ShaderType::Pixel, shaderTag);
	}
	if (gpSettingField.contains("il"))
	{
		shaderTag = gpSettingField["il"];
		mGPSetting.inputLayout = ShaderManager::GetInputLayout(shaderTag);
	}

	if (gpSettingField.contains("ds_setting"))
	{
		mGPSetting.depthStensilSetting.isDepthEnable = gpSettingField["ds_setting"]["is_depth_enable"];
		mGPSetting.depthStensilSetting.depthWriteMask = gpSettingField["ds_setting"]["depth_write_mask"];
		mGPSetting.depthStensilSetting.depthComparisonFunc = gpSettingField["ds_setting"]["depth_comparison_func"];
		mGPSetting.depthStensilSetting.dsvFormat = gpSettingField["ds_setting"]["dsv_format"];
	}

	// ルートシグネーチャ設定の読み込み
	nlohmann::json rsSettingField = deserialized["rs_setting"];
	mRSSetting.maxCbvRootParameter = rsSettingField["max_cbv"];
	mRSSetting.maxSrvDescritorRange = rsSettingField["max_srv"];
	mRSSetting.maxUavDescritorRange = rsSettingField["max_uav"];

	file.close();
}
