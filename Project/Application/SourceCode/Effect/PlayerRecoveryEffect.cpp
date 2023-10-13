#include "PlayerRecoveryEffect.h"

PlayerRecoveryEffect::PlayerRecoveryEffect() :
	mCircleEmitter(std::make_unique<Emitter>()),
	mLineEmitter(std::make_unique<Emitter>()),
	mIsGenerate(false)
{
}

void PlayerRecoveryEffect::Init()
{
	mCirclePParam.clear();
	mCircleEmitter->SetMaxParticle(2048);
	mCircleEmitter->SetTexture(TextureManager::GetTexture("Particle1"));

	mLinePParam.clear();
	mLineEmitter->SetMaxParticle(1024);
	mLineEmitter->SetTexture(TextureManager::GetTexture("Line"));
}

void PlayerRecoveryEffect::Generate(const Vec3 pos)
{
	mIsGenerate = true;
	mTimer.SetLimitTimer(20);
	mTimer.Reset();

	mStartPos = pos;
}

void PlayerRecoveryEffect::Update()
{
	if (mIsGenerate == true)
	{
		GenerateUpdate();
		mStartPos.y += 0.2f;

		mTimer.Update();
		if (mTimer == true)
		{
			mTimer.Reset();
			mIsGenerate = false;
		}
	}

	// 邊・
	for (uint32_t i = 0; i < mCirclePParam.size(); i++)
	{
		mCircleEmitter->pParam[i].curPos = mCirclePParam[i].startPos;
		mCirclePParam[i].startPos += mCirclePParam[i].moveVec * mCirclePParam[i].moveAccel;

		mCircleEmitter->pParam[i].curScale = mCirclePParam[i].startScale;
		mCirclePParam[i].startScale -= 0.005f;

		mCircleEmitter->pParam[i].curColor = mCirclePParam[i].startColor;
	}

	std::erase_if(mCirclePParam,
		[](ParticleParameter::PParam1 param)
		{
			return param.startScale <= 0;
		});

	mCircleEmitter->pSize = (uint32_t)mCirclePParam.size();

	// 邱・
	for (uint32_t i = 0; i < mLinePParam.size(); i++)
	{
		mLineEmitter->pParam[i].curPos = mLinePParam[i].startPos;
		mLinePParam[i].startPos += mLinePParam[i].moveVec * mLinePParam[i].moveAccel;

		mLineEmitter->pParam[i].curScale = mLinePParam[i].startScale;

		mLineEmitter->pParam[i].curColor = mLinePParam[i].startColor;
		mLinePParam[i].startColor.a -= 5.f;
	}

	std::erase_if(mLinePParam,
		[](ParticleParameter::PParam1 param)
		{
			return param.startColor.a <= 0;
		});

	mLineEmitter->pSize = (uint32_t)mLinePParam.size();


	mCircleEmitter->Update();
	mLineEmitter->Update();
}

void PlayerRecoveryEffect::DrawModel()
{
	mCircleEmitter->Draw();
	mLineEmitter->Draw();
}


void PlayerRecoveryEffect::GenerateUpdate()
{
	float width = 2;
	float height = 0.5f;
	float depth = 2;

	// 邊・
	for (uint32_t i = 0; i < 8; i++)
	{
		Vec3 offset =
		{
			Random::RangeF(-width,width),
			Random::RangeF(-height,height),
			Random::RangeF(-depth,depth),
		};

		mCirclePParam.emplace_back();
		mCirclePParam.back().startPos = mStartPos + offset;
		mCirclePParam.back().moveVec = Vec3::one;
		mCirclePParam.back().moveAccel =
		{
			Random::RangeF(-0.025f, 0.025f),
			0.05f,
			Random::RangeF(-0.025f, 0.025f)
		};
		mCirclePParam.back().startScale = Random::RangeF(0.15f, 0.35f);
		mCirclePParam.back().startColor = Color(0xffa80b);
	}

	width = 1.5f;
	height = 0.5f;
	depth = 1.5f;

	// 邱・
	for (uint32_t i = 0; i < 2; i++)
	{
		Vec3 offset =
		{
			Random::RangeF(-width,width),
			Random::RangeF(-height,height),
			Random::RangeF(-depth,depth),
		};

		mLinePParam.emplace_back();
		mLinePParam.back().startPos = mStartPos + offset;
		mLinePParam.back().moveVec = Vec3::one;
		mLinePParam.back().moveAccel = { 0.f,0.05f,0.f };
		mLinePParam.back().startScale = 1.5f;
		mLinePParam.back().startColor = Color(0xffe95c);
	}
}
