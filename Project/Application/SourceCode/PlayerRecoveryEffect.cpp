#include "PlayerRecoveryEffect.h"

PlayerRecoveryEffect::PlayerRecoveryEffect() :
	circleEmitter_(std::make_unique<Emitter>()),
	lineEmitter_(std::make_unique<Emitter>()),
	isGenerate(false)
{
	circleEmitter_->SetMaxParticle(2048);

	lineEmitter_->SetMaxParticle(1024);
	lineEmitter_->SetTexture(TextureManager::GetTexture("Line"));
}

void PlayerRecoveryEffect::Generate(const Vec3 pos)
{
	isGenerate = true;
	timer_.SetLimitTimer(20);
	timer_.Reset();

	startPos = pos;
}

void PlayerRecoveryEffect::Update()
{
	if (isGenerate == true)
	{
		GenerateUpdate();
		startPos.y += 0.2f;

		timer_.Update();
		if (timer_ == true)
		{
			timer_.Reset();
			isGenerate = false;
		}
	}

	// ó±
	for (uint32_t i = 0; i < circlePParam.size(); i++)
	{
		circleEmitter_->pParam[i].curPos = circlePParam[i].startPos;
		circlePParam[i].startPos += circlePParam[i].moveVec * circlePParam[i].moveAccel;

		circleEmitter_->pParam[i].curScale = circlePParam[i].startScale;
		circlePParam[i].startScale -= 0.005f;

		circleEmitter_->pParam[i].curColor = circlePParam[i].startColor;
	}

	std::erase_if(circlePParam,
		[](ParticleParameter::PParam1 param)
		{
			return param.startScale <= 0;
		});

	circleEmitter_->pSize = (uint32_t)circlePParam.size();

	// ê¸
	for (uint32_t i = 0; i < linePParam.size(); i++)
	{
		lineEmitter_->pParam[i].curPos = linePParam[i].startPos;
		linePParam[i].startPos += linePParam[i].moveVec * linePParam[i].moveAccel;

		lineEmitter_->pParam[i].curScale = linePParam[i].startScale;

		lineEmitter_->pParam[i].curColor = linePParam[i].startColor;
		linePParam[i].startColor.a -= 5.f;
	}

	std::erase_if(linePParam,
		[](ParticleParameter::PParam1 param)
		{
			return param.startColor.a <= 0;
		});

	lineEmitter_->pSize = (uint32_t)linePParam.size();


	circleEmitter_->Update();
	lineEmitter_->Update();
}

void PlayerRecoveryEffect::DrawModel()
{
	circleEmitter_->Draw();
	lineEmitter_->Draw();
}


void PlayerRecoveryEffect::GenerateUpdate()
{
	float width = 2;
	float height = 0.5f;
	float depth = 2;

	// ó±
	for (uint32_t i = 0; i < 8; i++)
	{
		Vec3 offset =
		{
			Random::RangeF(-width,width),
			Random::RangeF(-height,height),
			Random::RangeF(-depth,depth),
		};

		circlePParam.emplace_back();
		circlePParam.back().startPos = startPos + offset;
		circlePParam.back().moveVec = Vec3::one;
		circlePParam.back().moveAccel =
		{
			Random::RangeF(-0.025f, 0.025f),
			0.05f,
			Random::RangeF(-0.025f, 0.025f)
		};
		circlePParam.back().startScale = Random::RangeF(0.15f, 0.35f);
		circlePParam.back().startColor = Color(0xffa80b);
	}

	width = 1.5f;
	height = 0.5f;
	depth = 1.5f;

	// ê¸
	for (uint32_t i = 0; i < 2; i++)
	{
		Vec3 offset =
		{
			Random::RangeF(-width,width),
			Random::RangeF(-height,height),
			Random::RangeF(-depth,depth),
		};

		linePParam.emplace_back();
		linePParam.back().startPos = startPos + offset;
		linePParam.back().moveVec = Vec3::one;
		linePParam.back().moveAccel = { 0.f,0.05f,0.f };
		linePParam.back().startScale = 1.5f;
		linePParam.back().startColor = Color(0xffe95c);
	}
}