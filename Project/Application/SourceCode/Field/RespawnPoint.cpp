#include "RespawnPoint.h"
#include "EffectManager.h"

RespawnPoint::RespawnPoint() :
	mRipple(std::make_unique<Object3D>()),
	mRhombus(std::make_unique<Object3D>())
{
	mRipple->SetModel(ModelManager::GetModel("Plane"));
	mRipple->SetTexture(TextureManager::GetTexture("DissolveTexture"));

	mRhombus->SetModel(ModelManager::GetModel("Plane"));
	mRhombus->SetTexture(TextureManager::GetTexture("BlurNoice"));
}

void RespawnPoint::Init()
{
	mRipple->pos.y = 0.05f;
	mRipple->rot.x = Radian(90);
	mRipple->scale = 2.f;
	mRipple->color = Color(0xc4c178);
	mRipple->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Ripple"));

	mRhombus->pos.y = 1.5f;
	mRhombus->color = Color(0xc4c178);
	mRhombus->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Rhombus"));
	mRhombus->SetBillboardType(BillboardType::YAxisBillboard);

	mAngle.SetLimitTimer(360);

	EffectManager::GetInstance()->GenerateLeadEffect(mParent.pos + Vec3::up * 5.f, Vec3::front + Vec3(0, 0.5, 0));
}

void RespawnPoint::Update()
{
	ColliderUpdatge();

	mRipple->offset.x -= 0.001f;
	mRhombus->offset.x += 0.01f;

	mRhombus->pos.y += cosf(Radian((float)mAngle.GetTimer())) * 0.0075f;
	mAngle.Update();
	if (mAngle == true)
	{
		mAngle.Reset();
	}

	mRipple->Update(&mParent);
	mRhombus->Update(&mParent);
}

void RespawnPoint::DrawModel()
{
	mRipple->Draw();
	mRhombus->Draw();
}

void RespawnPoint::ColliderUpdatge()
{
	mCollider.centerPos = mParent.pos;
	mCollider.radius = mParent.scale.x;
}

void RespawnPoint::SetParent(const Transform parent)
{
	mParent = parent;
	mParent.Update();
}

void RespawnPoint::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mRipple->SetGraphicsPipeline(graphicsPipeline);
	mRhombus->SetGraphicsPipeline(graphicsPipeline);
}

Vec3 RespawnPoint::GetPos()
{
	return mParent.pos;
}

SphereCollider RespawnPoint::GetCollider()
{
	return mCollider;
}

