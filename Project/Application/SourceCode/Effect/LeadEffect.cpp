#include "LeadEffect.h"

LeadEffect::LeadEffect() :
	mEmitter(std::make_unique<Emitter>())
{
}

void LeadEffect::Init()
{
	mPParam.clear();
	mEmitter->SetMaxParticle(256);
	mEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mTimer.SetLimitTimer(20);
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
	// 邊・
	for (uint32_t i = 0; i < mPParam.size(); i++)
	{
		mPParam[i].ease.Update();

		if (mPParam[i].ease.GetisEnd() == false)
		{
			if (mPParam[i].ease.GetTimer() % 200 == 0)
			{
				mPParam.back().moveAccel =
				{
					Random::RangeF(0.02f, 0.04f),
					Random::RangeF(-0.0125f, 0.0125f),
					Random::RangeF(0.02f, 0.04f)
				};

				Vec3 vec = mFrontVec;

				// 蜿ｳ繝吶け繝医Ν繧呈ｱゅａ繧・
				Vec3 rightVec = Vec3::Cross(mFrontVec, Vec3::up);

				// 繧ｯ繧ｩ繝ｼ繧ｿ繝九が繝ｳ繧剃ｽｿ縺｣縺ｦ蜿ｳ繝吶け繝医Ν繧貞渕貅悶↓縺ｾ縺壻ｸ贋ｸ九↓蝗櫁ｻ｢
				Quaternion q1;
				q1 = vec;
				vec = q1.AnyAxisRotation(rightVec, Radian(Random::RangeAngle(-15, 15)));

				// 蜑阪・繧ｯ繝医Ν繧貞渕貅悶↓繧ゅ≧荳蝗槫屓霆｢縺吶ｋ
				Quaternion q2;
				q2 = vec;
				vec = q2.AnyAxisRotation(mFrontVec, Random::RangeRadian());

				mPParam.back().moveVec = vec;
			}
		}
		else
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

	// 邊・
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

		// 蜿ｳ繝吶け繝医Ν繧呈ｱゅａ繧・
		Vec3 rightVec = Vec3::Cross(mFrontVec, Vec3::up);

		// 繧ｯ繧ｩ繝ｼ繧ｿ繝九が繝ｳ繧剃ｽｿ縺｣縺ｦ蜿ｳ繝吶け繝医Ν繧貞渕貅悶↓縺ｾ縺壻ｸ贋ｸ九↓蝗櫁ｻ｢
		Quaternion q1;
		q1 = vec;
		vec = q1.AnyAxisRotation(rightVec, Radian(Random::RangeAngle(-15, 15)));

		// 蜑阪・繧ｯ繝医Ν繧貞渕貅悶↓繧ゅ≧荳蝗槫屓霆｢縺吶ｋ
		Quaternion q2;
		q2 = vec;
		vec = q2.AnyAxisRotation(mFrontVec, Random::RangeRadian());

		mPParam.back().moveVec = vec;// Vec3::one;
		mPParam.back().moveAccel =
		{
			Random::RangeF(0.02f, 0.04f),
			0.0125f,
			Random::RangeF(0.02f, 0.04f)
		};
		mPParam.back().startScale = Random::RangeF(0.25f, 0.35f);
		mPParam.back().startColor = Color(0xc4c178);
		mPParam.back().startShininess = 1.5f;

		mPParam.back().ease.SetEaseTimer(1200);
	}
}
