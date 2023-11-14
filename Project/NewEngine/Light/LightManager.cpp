#include "LightManager.h"
#include "NewEngineEnum.h"
#include "ConstantBufferData.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

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

void LightManager::Init()
{
	mLightGroup.directionalLights.clear();
	mLightGroup.pointLights.clear();
	mLightGroup.spotLights.clear();
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

		// 座標
		lightGroupData.pointLightsData[i].pos = light->pos;

		// 影響範囲
		lightGroupData.pointLightsData[i].radius = light->radius;

		// 色の影響率
		lightGroupData.pointLightsData[i].colorRate = light->colorRate;

		// 減衰係数
		lightGroupData.pointLightsData[i].decay = light->decay;

		// アクティブフラグ
		lightGroupData.pointLightsData[i].isActive = light->isActive;
	}

	// スポットライトのデータを転送する

	for (uint32_t i = 0; i < mLightGroup.spotLights.size(); i++)
	{
		// 宣言したライトの数よりも多かったらbreakする
		if (i > SpotLightSize)
		{
			OutputDebugLog("[Spot Light] warring : Not enough Spot Lights");
			break;
		}

		SpotLight* light = dynamic_cast<SpotLight*>(mLightGroup.spotLights[i]);

		// 色
		lightGroupData.spotLightsData[i].color = light->color.To01();

		// ベクトル
		lightGroupData.spotLightsData[i].vec = -light->vec.Norm();

		// 影響範囲
		lightGroupData.spotLightsData[i].radius = light->radius;

		// 座標
		lightGroupData.spotLightsData[i].pos = light->pos;

		// 減衰係数
		lightGroupData.spotLightsData[i].decay = light->decay;

		// 色の影響率
		lightGroupData.spotLightsData[i].colorRate = light->colorRate;

		// アクティブフラグ
		lightGroupData.spotLightsData[i].isActive = light->isActive;

		// 角度減衰
		lightGroupData.spotLightsData[i].cosAngle.x = cosf(Radian(light->cosAngle.x));
		lightGroupData.spotLightsData[i].cosAngle.y = cosf(Radian(light->cosAngle.y));
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

	case LightType::SpotLight:
		// 点光源に登録
		mLightGroup.spotLights.push_back(iLight);
		break;
	}
}
