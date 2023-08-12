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

		mTransition[i]->color = Color(0xc4c178);
	}
}

void RespawnTransition::Generate()
{
	for (uint32_t i = 0; i < mTransition.size(); i++)
	{
		mTransition[i]->pos = GetWindowHalfSize();

		mTransitionData[i].min = 0.01f;
		mTransitionData[i].max = 0.01f;

		mUVParameteData[i].offset = 0;
		mUVParameteData[i].tiling = 1;
	}
	mTransition[Back]->scale = 2;
	mTransition[Front]->scale = 4;
	mUVParameteData[Front].offset.x = 0.25f;

	mStep = None;
}

void RespawnTransition::Update()
{
	const float transitionDataAccel = 0.01f;

	switch (mStep)
	{
	case In:
	{
		mUVParameteData[Back].offset.y -= 0.001f;

		mTransitionData[Front].max += transitionDataAccel;
		mTransitionData[Back].max += transitionDataAccel * 0.5f;

		if (mTransitionData[Back].max >= 1.f)
		{
			mStep = Progress;
		}
	}
	break;

	case Out:
	{
		mUVParameteData[Back].offset.y -= 0.001f;

		mTransitionData[Front].max -= transitionDataAccel;
		mTransitionData[Back].max -= transitionDataAccel * 0.85f;

		if (mTransitionData[Back].max <= 0.f)
		{
			mStep = End;
		}
	}
	break;
	}
	mTransition[Front]->rot -= Radian(0.1f);

	for (uint32_t i = 0; i < mTransition.size(); i++)
	{
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

RespawnTransition::Step RespawnTransition::GetStep()
{
	return mStep;
}

void RespawnTransition::SetStep(const Step step)
{
	mStep = step;
}
