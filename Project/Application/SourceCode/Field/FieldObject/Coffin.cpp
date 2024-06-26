#include "Coffin.h"

Coffin::Coffin() :
	mCoffinTop(std::make_unique<Object3D>()),
	mCoffinBottom(std::make_unique<Object3D>())
{
	mCoffinTop->SetModel(ModelManager::GetModel("CoffinTop"));
	mCoffinBottom->SetModel(ModelManager::GetModel("CoffinBottom"));

	mBottomCollider.isActive = false;

	mType = FieldObjectType::Coffin;
}
void Coffin::Init()
{
	mCoffinTop->SetisShadow(true, true);
	mCoffinBottom->SetisShadow(true, true);
	mCoffinTop->isLighting = true;
	mCoffinBottom->isLighting = true;

	mCoffinTop->pos = 0;
}
void Coffin::Update()
{
	ColliderDrawer::GetInstance()->Bind(&mBottomCollider);

	mCoffinTop->SetParent(&mParent);
	mCoffinBottom->SetParent(&mParent);

	mCoffinTop->Update();
	mCoffinBottom->Update();
}
void Coffin::Draw(const bool isDrawDepth)
{
	if (isDrawDepth == false)
	{
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
	}
	else
	{
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	}

	mCoffinTop->Draw("Object3D");
	mCoffinBottom->Draw("Object3D");
}

// セッター
void Coffin::SetParent(const Transform parent)
{
	mParent = parent;
	mParent.Update();
}
void Coffin::SetTopPos(const Vec3 pos)
{
	mCoffinTop->pos = pos;
}
void Coffin::SetBottomPos(const Vec3 pos)
{
	mCoffinBottom->pos = pos;
}
void Coffin::SetTopRot(const Vec3 rot)
{
	mCoffinTop->rot = rot;
}
void Coffin::SetBottomRot(const Vec3 rot)
{
	mCoffinBottom->rot = rot;
}
void Coffin::SetBottomCollider(const CubeCollider& collider)
{
	mBottomCollider = collider;
	mBottomCollider.CalcPoints();
	mBottomCollider.isActive = true;
}
void Coffin::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mCoffinTop->SetGraphicsPipeline(graphicsPipeline);
	mCoffinBottom->SetGraphicsPipeline(graphicsPipeline);
}

// ゲッター
CubeCollider Coffin::GetBottomCollider()
{
	return mBottomCollider;
}
Vec3 Coffin::GetPos()
{
	return mParent.pos;
}
Vec3 Coffin::GetTopPos()
{
	return mCoffinTop->pos;
}
