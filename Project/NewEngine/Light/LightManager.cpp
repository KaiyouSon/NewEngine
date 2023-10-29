#include "LightManager.h"
#include "NewEngineEnum.h"
#include "ConstantBufferData.h"
#include "DirectionalLight.h"
#include "PointLight.h"

using namespace ConstantBufferData;

LightManager::LightManager() :
	mMaterial(std::make_unique<Material>())
{
	// マテリアルの初期化
	MaterialInit();
}

void LightManager::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// ライトグループのデータ
	iConstantBuffer = std::make_unique<ConstantBuffer<CLightGroup>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	mMaterial->Init();
}

void LightManager::Update()
{
	// ライトグループのデータ
	CLightGroup lightGroupData;

	// 平行光源のデータを転送する
	for (uint32_t i = 0; i < mLightGroup.directionalLights.size(); i++)
	{
		// 宣言したライトの数よりも多かったらbreakする
		if (i > DirectionalLightSize)
		{
			OutputDebugLog("[Direction Light] warring : Not enough Directional Lights");
			break;
		}

		DirectionalLight* light = dynamic_cast<DirectionalLight*>(mLightGroup.directionalLights[i]);

		// 色
		lightGroupData.directionalLightsData[i].color = light->color.To01();
		// ベクトル
		lightGroupData.directionalLightsData[i].vec = light->pos.Norm();
		// アクティブフラグ
		lightGroupData.directionalLightsData[i].isActive = light->isActive;
	}

	// 点光源のデータを転送する
	for (uint32_t i = 0; i < mLightGroup.pointLights.size(); i++)
	{
		// 宣言したライトの数よりも多かったらbreakする
		if (i > PointLightSize)
		{
			OutputDebugLog("[Point Light] warring : Not enough Point Lights");
			break;
		}

		PointLight* light = dynamic_cast<PointLight*>(mLightGroup.pointLights[i]);

		// 色
		lightGroupData.pointLightsData[i].color = light->color.To01();
		// 影響範囲
		lightGroupData.pointLightsData[i].radius = light->radius;
		// ベクトル
		lightGroupData.pointLightsData[i].pos = light->pos;
		// 減衰係数
		lightGroupData.pointLightsData[i].colorRate = light->colorRate;
		// アクティブフラグ
		lightGroupData.pointLightsData[i].isActive = light->isActive;
	}

	// データ転送
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), lightGroupData);
}
void LightManager::DrawCommand(const uint32_t index)
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		index, mMaterial->constantBuffers[0]->bufferResource->buffer->GetGPUVirtualAddress());
}

void LightManager::Register(ILight* iLight)
{
	// インターフェースのタイプよって登録先を決める

	switch (iLight->GetLightType())
	{
	case LightType::DirectionalLight:
		// 平行光源に登録
		mLightGroup.directionalLights.push_back(iLight);
		break;

	case LightType::PointLight:
		// 点光源に登録
		mLightGroup.pointLights.push_back(iLight);
		break;

	}
}
