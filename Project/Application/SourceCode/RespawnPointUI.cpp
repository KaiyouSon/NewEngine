#include "RespawnPointUI.h"
using namespace ConstantBufferData;

void RespawnPointUI::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	ShaderObjectManager::Create("RespawnTransition");
	ShaderObjectManager::GetShaderObject("RespawnTransition")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("RespawnTransition")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("RespawnTransition")->CompileVertexShader(path + "RespawnTransitionVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("RespawnTransition")->CompilePixelShader(path + "RespawnTransitionPS.hlsl", "main");

	GraphicsPipelineSetting setting = GraphicsPipelineManager::GetGraphicsPipeline("Sprite")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("RespawnTransition");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.constantBufferViewNum = 4;
	GraphicsPipelineManager::Create(setting, "RespawnTransition");
}

RespawnPointUI::RespawnPointUI() :
	mBack(std::make_unique<Sprite>())
{


	mBack->SetTexture(TextureManager::GetTexture("RespawnBack"));

	for (uint32_t i = 0; i < mText.size(); i++)
	{
		mText[i] = std::make_unique<Sprite>();
		mText[i]->SetTexture(TextureManager::GetTexture("DecisionCloseStr"));
		mText[i]->scale = 0.25f;
	}

	Init();
}

void RespawnPointUI::Init()
{
	mBack->pos.x = -4;
	mBack->scale = 2;

	mParent.pos = Vec2(480, GetWindowHalfSize().y);
	mParent.Update();
}

void RespawnPointUI::Update()
{
	const float add = mIsActive ? 35.f : -35.f;
	mAlpha += add;
	mAlpha = Clamp<float>(mAlpha, 0.f, 255.f);

	mBack->color.a = mAlpha;

	mBack->Update(&mParent);
	for (uint32_t i = 0; i < mText.size(); i++)
	{
		mText[i]->pos = Vec2(-196, 1080 / 2 - 32);
		mText[i]->Update(&mParent);
	}
}

void RespawnPointUI::DrawFrontSprite()
{
	if (mAlpha == 0)
	{
		return;
	}

	mBack->Draw();
	for (uint32_t i = 0; i < mText.size(); i++)
	{
		mText[i]->Draw();
	}
}

void RespawnPointUI::SetisActive(const bool isActive)
{
	mIsActive = isActive;
}

bool RespawnPointUI::GetisActive()
{
	return mIsActive;
}
