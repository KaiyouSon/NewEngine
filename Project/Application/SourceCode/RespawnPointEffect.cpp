#include "RespawnPointEffect.h"

RespawnPointEffect::RespawnPointEffect() :
	//mCircleEmitter(std::make_unique<Emitter>())
	mCircleEmitter(std::make_unique<GPUEmitter>())
{
}

void RespawnPointEffect::Init()
{
	mCirclePParam.clear();
	mCircleEmitter->SetMaxParticle(32);
	mCircleEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mTimer.SetLimitTimer(20);
	mTimer.Reset();
}

void RespawnPointEffect::Generate(const Vec3 pos)
{
	mStartPos = pos;
	if (mTimer == true)
	{
		GenerateUpdate();
		mTimer.Reset();
	}
	mTimer.Update();
}

void RespawnPointEffect::Update()
{
	// —±
	for (uint32_t i = 0; i < mCirclePParam.size(); i++)
	{
		mCircleEmitter->pParam[i].curPos = mCirclePParam[i].startPos;
		mCirclePParam[i].startPos += mCirclePParam[i].moveVec * mCirclePParam[i].moveAccel;

		mCircleEmitter->pParam[i].curScale = mCirclePParam[i].startScale;
		mCirclePParam[i].startScale -= 0.001f;

		mCircleEmitter->pParam[i].curShininess = mCirclePParam[i].startShininess;

		mCircleEmitter->pParam[i].curColor = mCirclePParam[i].startColor;
	}

	std::erase_if(mCirclePParam,
		[](ParticleParameter::PParam1 param)
		{
			return param.startScale <= 0;
		});

	mCircleEmitter->pSize = (uint32_t)mCirclePParam.size();
	mCircleEmitter->Update();
}

void RespawnPointEffect::DrawModel()
{
	mCircleEmitter->Draw();
}

void RespawnPointEffect::GenerateUpdate()
{
	float width = 2.0f;
	float height = 1.0f;
	float depth = 2.0f;

	// —±
	for (uint32_t i = 0; i < 1; i++)
	{
		Vec3 offset =
		{
			Random::RangeF(-width,width),
			Random::RangeF(0.0f,height),
			Random::RangeF(-depth,depth),
		};

		mCirclePParam.emplace_back();
		mCirclePParam.back().startPos = mStartPos + offset;
		mCirclePParam.back().moveVec = Vec3::one;
		mCirclePParam.back().moveAccel =
		{
			Random::RangeF(-0.01f, 0.01f),
			0.0125f,
			Random::RangeF(-0.01f, 0.01f)
		};
		mCirclePParam.back().startScale = Random::RangeF(0.35f, 0.45f);
		mCirclePParam.back().startColor = Color(0xc4c178);
		mCirclePParam.back().startShininess = 1.5f;
	}
}