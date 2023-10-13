#include "RespawnPointEffect.h"
using namespace StructuredBufferData;
using namespace ParticleParameter;

RespawnPointEffect::RespawnPointEffect() :
	//mCircleEmitter(std::make_unique<Emitter>())
	mCircleEmitter(std::make_unique<GPUEmitter>())
{
}

void RespawnPointEffect::Init()
{
	mCirclePParam.clear();
	mCircleEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("RespawnPointEffect"));
	mCircleEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("RespawnPointEffect"));
	mCircleEmitter->SetParticleData<RespawnPointParticle>(64);
	mCircleEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mCircleEmitter->AddStructuredBuffer<STimer>();
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
	//mCircleEmitter->pos.x += (float)(Key::GetKey(DIK_RIGHT) - Key::GetKey(DIK_LEFT));
	//mCircleEmitter->pos.z += (float)(Key::GetKey(DIK_UP) - Key::GetKey(DIK_DOWN));

	// 邊・
	//for (uint32_t i = 0; i < mCirclePParam.size(); i++)
	//{
	//	mCircleEmitter->pParam[i].curPos = mCirclePParam[i].startPos;
	//	mCirclePParam[i].startPos += mCirclePParam[i].moveVec * mCirclePParam[i].moveAccel;

	//	mCircleEmitter->pParam[i].curScale = mCirclePParam[i].startScale;
	//	mCirclePParam[i].startScale -= 0.001f;

	//	mCircleEmitter->pParam[i].curShininess = mCirclePParam[i].startShininess;

	//	mCircleEmitter->pParam[i].curColor = mCirclePParam[i].startColor;
	//}

	//std::erase_if(mCirclePParam,
	//	[](ParticleParameter::PParam1 param)
	//	{
	//		return param.startScale <= 0;
	//	});

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

	// 邊・
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
