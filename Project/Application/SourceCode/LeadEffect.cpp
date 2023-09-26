#include "LeadEffect.h"

LeadEffect::LeadEffect() :
	mEmitter(std::make_unique<Emitter>())
{
}

void LeadEffect::Init()
{
	mEmitter->SetMaxParticle(64);
	mEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mTimer.SetLimitTimer(10);
	mTimer.Reset();
}

void LeadEffect::Generate(const Vec3 pos, const Vec3 frontVec)
{
	mStartPos = pos;
	mFrontVec = frontVec;

	if (mTimer == true)
	{
		GenerateUpdate();
		mTimer.Reset();
	}
	mTimer.Update();
}

void LeadEffect::Update()
{
	// 粒
	for (uint32_t i = 0; i < mPParam.size(); i++)
	{
		mPParam[i].ease.Update();
		if (mPParam[i].ease.GetisEnd() == true)
		{
			mPParam[i].startScale -= 0.01f;
		}
		mEmitter->pParam[i].curScale = mPParam[i].startScale;

		mEmitter->pParam[i].curPos = mPParam[i].startPos;
		mPParam[i].startPos += mPParam[i].moveVec * mPParam[i].moveAccel;

		mEmitter->pParam[i].curShininess = mPParam[i].startShininess;

		mEmitter->pParam[i].curColor = mPParam[i].startColor;
	}

	std::erase_if(mPParam,
		[](ParticleParameter::PParam1 param)
		{
			return param.startScale <= 0;
		});

	mEmitter->pSize = (uint32_t)mPParam.size();
	mEmitter->Update();
}

void LeadEffect::DrawModel()
{
	mEmitter->Draw();
}

void LeadEffect::GenerateUpdate()
{
	if (mPParam.size() >= 64)
	{
		return;
	}

	float width = 0.5f;
	float height = 0.5f;
	float depth = 0.5f;

	// 粒
	for (uint32_t i = 0; i < 1; i++)
	{
		Vec3 offset =
		{
			Random::RangeF(-width,width),
			Random::RangeF(-height,height),
			Random::RangeF(-depth,depth),
		};

		mPParam.emplace_back();
		mPParam.back().startPos = mStartPos + offset;

		Vec3 vec = mFrontVec;

		// 右ベクトルを求める
		Vec3 rightVec = Vec3::Cross(mFrontVec, Vec3::up);

		// クォータニオンを使って右ベクトルを基準にまず上下に回転
		Quaternion q1;
		q1 = vec;
		vec = q1.AnyAxisRotation(rightVec, Radian(Random::RangeAngle(-30, 30)));

		// 前ベクトルを基準にもう一回回転する
		Quaternion q2;
		q2 = vec;
		vec = q2.AnyAxisRotation(mFrontVec, Random::RangeRadian());

		mPParam.back().moveVec = vec;// Vec3::one;
		mPParam.back().moveAccel =
		{
			Random::RangeF(0.01f, 0.02f),
			0.0125f,
			Random::RangeF(0.01f, 0.02f)
		};
		mPParam.back().startScale = Random::RangeF(0.35f, 0.45f);
		mPParam.back().startColor = Color(0xc4c178);
		mPParam.back().startShininess = 1.5f;

		mPParam.back().ease.SetEaseTimer(300);
	}
}
