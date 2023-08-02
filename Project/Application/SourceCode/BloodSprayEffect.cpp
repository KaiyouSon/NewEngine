#include "BloodSprayEffect.h"

BloodSprayEffect::BloodSprayEffect() :
	mEmitter(std::make_unique<Emitter>())
{
	mEmitter->SetMaxParticle(1024);
}

void BloodSprayEffect::Generate(const Vec3 pos)
{
	for (uint32_t i = 0; i < 256; i++)
	{
		mPParam.emplace_back();
		float radian = Radian(Random::RangeAngle(1, 360));

		mPParam.back().startPos = pos;
		mPParam.back().moveVec = { cosf(radian), Random::RangeF(1,2),sinf(radian) };
		mPParam.back().moveAccel =
		{
			Random::RangeF(0.05f,0.15f),
			Random::RangeF(0.1f,0.3f),
			Random::RangeF(0.05f,0.15f)
		};
		mPParam.back().startColor = Color::red;
	}
}

void BloodSprayEffect::Update()
{
	for (uint32_t i = 0; i < mPParam.size(); i++)
	{
		mPParam[i].moveAccel.y -= 0.01f;

		mPParam[i].startPos += mPParam[i].moveVec * mPParam[i].moveAccel;
		mEmitter->pParam[i].curPos = mPParam[i].startPos;

		mEmitter->pParam[i].curScale = 0.25f;

		mPParam[i].startColor.a -= 5.f;
		mEmitter->pParam[i].curColor = mPParam[i].startColor;
	}

	std::erase_if(mPParam,
		[](ParticleParameter::PParam1 param)
		{
			return param.startColor.a <= 0;
		});

	mEmitter->pSize = (uint32_t)mPParam.size();
	mEmitter->Update();
}

void BloodSprayEffect::DrawModel()
{
	mEmitter->Draw();
}
