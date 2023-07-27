#include "BloodSpray.h"

BloodSpray::BloodSpray() :
	emitter_(std::make_unique<Emitter>())
{
	emitter_->SetMaxParticle(1024);
}

void BloodSpray::Generate(const Vec3 pos)
{
	for (uint32_t i = 0; i < 128; i++)
	{
		pParam.emplace_back();
		float radian = Radian(Random::RangeAngle(30, 150));

		pParam.back().startPos = pos;
		pParam.back().moveVec = { cosf(radian), sinf(radian), 0 };
		pParam.back().moveAccel = { Random::RangeF(0.05f,0.15f), Random::RangeF(0.1f,0.3f), 0 };
		pParam.back().startColor = Color::red;
	}
}

void BloodSpray::Update()
{
	for (uint32_t i = 0; i < pParam.size(); i++)
	{
		pParam[i].moveAccel.y -= 0.01f;

		pParam[i].startPos.x += pParam[i].moveVec.x * pParam[i].moveAccel.x;
		pParam[i].startPos.y += pParam[i].moveVec.y * pParam[i].moveAccel.y;
		emitter_->pParam[i].curPos = pParam[i].startPos;

		//emitter_->pParam[i].curScale = 0.25f;
		emitter_->pParam[i].curScale = 1.25f;

		pParam[i].startColor.a -= 5.f;
		emitter_->pParam[i].curColor = pParam[i].startColor;
	}

	std::erase_if(pParam,
		[](ParticleParameter::PParam1 param)
		{
			return param.startColor.a <= 0;
		});

	emitter_->pSize = (uint32_t)pParam.size();
	emitter_->Update();
}

void BloodSpray::DrawModel()
{
	emitter_->Draw();
}
