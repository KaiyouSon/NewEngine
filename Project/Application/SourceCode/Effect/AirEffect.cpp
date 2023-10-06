#include "AirEffect.h"

AirEffect::AirEffect() :
	mEmitter(std::make_unique<Emitter>())
{
}

void AirEffect::Init()
{
	maxParticle = 512;

	mPParam.clear();
	mEmitter->SetMaxParticle(maxParticle);
	mEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mTimer.SetLimitTimer(60);
	mTimer.Reset();
}

void AirEffect::Generate(const Vec3 pos)
{
	mStartPos = pos;

	if (mTimer == true)
	{
		GenerateUpdate();
		mTimer.Reset();
	}
	mTimer.Update();
}

void AirEffect::Update()
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
					Random::RangeF(-0.02f, 0.02f),
					Random::RangeF(-0.05f, -0.025f),
					Random::RangeF(-0.02f, 0.02f)
				};
			}
		}
		else
		{
			mPParam[i].startColor.a -= 1.f;
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
			return param.startColor.a <= 0;
		});

	mEmitter->pSize = (uint32_t)mPParam.size();
	mEmitter->Update();
}

void AirEffect::DrawModel()
{
	mEmitter->Draw();
}

void AirEffect::GenerateUpdate()
{
	if (mPParam.size() >= maxParticle)
	{
		return;
	}

	float width = 100.f;
	float height = 20.f;
	float depth = 100.f;

	// 邊・
	for (uint32_t i = 0; i < 10; i++)
	{
		Vec3 offset =
		{
			Random::RangeF(-width,width),
			Random::RangeF(height,height * 2),
			Random::RangeF(-depth,depth),
		};

		mPParam.emplace_back();
		mPParam.back().startPos = mStartPos + offset;

		mPParam.back().moveVec = Vec3::one;
		mPParam.back().moveAccel =
		{
			Random::RangeF(-0.02f, 0.02f),
			Random::RangeF(-0.05f, -0.025f),
			Random::RangeF(-0.02f, 0.02f)
		};
		mPParam.back().startScale = Random::RangeF(0.25f, 0.35f);

		switch (Random::Range(1, 3))
		{
		case 1:
			mPParam.back().startColor = Color::white;
			break;
		case 2:
			mPParam.back().startColor = Color(0xffdc55);
			break;
		case 3:
			mPParam.back().startColor = Color(0xff8148);
			break;

		}

		mPParam.back().startShininess = 1.5f;

		mPParam.back().ease.SetEaseTimer(1200);
	}
}

