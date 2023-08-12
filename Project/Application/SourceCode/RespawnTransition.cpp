#include "RespawnTransition.h"
using namespace ConstantBufferData;

RespawnTransition::RespawnTransition()
{
	for (uint32_t i = 0; i < mTransition.size(); i++)
	{
		mTransition[i] = std::make_unique<Sprite>();
		mTransition[i]->SetTexture(TextureManager::GetTexture("ScreenNoice"));
		mTransition[i]->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("RespawnTransition"));
		mTransition[i]->AddMaterial(ConstantBuffer<CRespawnTransition>());
		mTransition[i]->AddMaterial(ConstantBuffer<CUVParameter>());
	}
}

void RespawnTransition::Generate()
{
	for (uint32_t i = 0; i < mTransition.size(); i++)
	{
		mTransition[i]->pos = GetWindowHalfSize();

		mTransitionData[i].min = 0.01f;
		mTransitionData[i].max = 1.0f;

		mUVParameteData[i].offset = 0;
		mUVParameteData[i].tiling = 1;
	}
	mTransition[Back]->scale = 2;
	mTransition[Front]->scale = 4;
	mUVParameteData[Front].offset.x = 0.25f;
}

void RespawnTransition::Update()
{
	mUVParameteData[Back].offset.y -= 0.001f;
	mTransition[Front]->rot -= Radian(1);

	for (uint32_t i = 0; i < mTransition.size(); i++)
	{
		if (Key::GetKey(DIK_UP))
		{
			mTransitionData[i].max += 0.01f;
		}
		if (Key::GetKey(DIK_DOWN))
		{
			mTransitionData[i].max -= 0.01f;
		}

		mTransitionData[i].max = Clamp(mTransitionData[i].max, mTransitionData[i].min, 1.0f);

		mTransition[i]->SetTransferBuffer(2, mTransitionData[i]);
		mTransition[i]->SetTransferBuffer(3, mUVParameteData[i]);
		mTransition[i]->Update();
	}
}

void RespawnTransition::DrawFrontSprite()
{
	for (uint32_t i = 0; i < mTransition.size(); i++)
	{
		mTransition[i]->Draw();
	}
}