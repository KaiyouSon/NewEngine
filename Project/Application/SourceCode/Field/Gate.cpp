#include "Gate.h"

Gate::Gate() :
	mGateLeft(std::make_unique<Object3D>()),
	mGateRight(std::make_unique<Object3D>())
{
	mGateLeft->SetModel(ModelManager::GetModel("WallGate"));
	mGateRight->SetModel(ModelManager::GetModel("WallGate"));
}

void Gate::Init()
{
	mGateLeft->SetisShadow(false, true);
	mGateRight->SetisShadow(false, true);
	mGateLeft->isLighting = true;
	mGateRight->isLighting = true;

	mLeftCollider.isActive = false;
	mRightCollider.isActive = false;
	mCloseCollider.isActive = true;
	mNegotiationCollider.isActive = false;
	mIsOpen = false;

	mOpenEase.SetEaseTimer(180);
	mStayTimer.SetLimitTimer(30);

	mGateLeft->rot.y = 0;
	mGateRight->rot.y = Radian(180);
}

void Gate::Update()
{
	mNegotitationPos = mCenterPos + Vec3::back * 7.5f;

	ColliderUpdate();
	OpeningUpdate();

	mGateLeft->Update();
	mGateRight->Update();
}

void Gate::DrawModel()
{
	mGateLeft->Draw();
	mGateRight->Draw();
}

void Gate::ColliderUpdate()
{
	const float endLength = 20.f;

	const Vec3 leftVec = { cosf(mGateLeft->rot.y), 0, -sinf(mGateLeft->rot.y) };
	mLeftCollider.startPos = mGateLeft->pos;
	mLeftCollider.endPos = mGateLeft->pos + leftVec.Norm() * endLength;
	mLeftCollider.radius = 5.f;

	const Vec3 rightVec = { cosf(mGateRight->rot.y), 0, -sinf(mGateRight->rot.y) };
	mRightCollider.startPos = mGateRight->pos;
	mRightCollider.endPos = mGateRight->pos + rightVec.Norm() * endLength;
	mRightCollider.radius = 5.f;

	mCloseCollider.startPos = mGateLeft->pos;
	mCloseCollider.endPos = mGateRight->pos;
	mCloseCollider.radius = 5.f;

	if (mIsOpen == true)
	{
		mLeftCollider.isActive = true;
		mRightCollider.isActive = true;
		mCloseCollider.isActive = false;
		mNegotiationCollider.isActive = false;
	}
	else
	{
		mLeftCollider.isActive = false;
		mRightCollider.isActive = false;
		mCloseCollider.isActive = true;
		mNegotiationCollider.isActive = true;
	}

	ColliderDrawer::GetInstance()->Bind(&mLeftCollider);
	ColliderDrawer::GetInstance()->Bind(&mRightCollider);
	ColliderDrawer::GetInstance()->Bind(&mCloseCollider);
	ColliderDrawer::GetInstance()->Bind(&mNegotiationCollider);
}

void Gate::OpeningUpdate()
{
	if (mIsOpening == false)
	{
		return;
	}

	mStayTimer.Update();
	if (mStayTimer == true)
	{
		mGateLeft->rot.y = mOpenEase.Lerp(Radian(0), Radian(-45));
		mGateRight->rot.y = mOpenEase.Lerp(Radian(180), Radian(225));

		mOpenEase.Update();
		if (mOpenEase.GetisEnd() == true)
		{
			mIsOpen = true;
			mIsOpening = false;
			mOpenEase.Reset();
			mStayTimer.Reset();
		}
	}
}

void Gate::SetLeftTransform(const Transform& transform)
{
	mGateLeft->pos = transform.pos;
	mGateLeft->scale = transform.scale;
	mGateLeft->rot = transform.rot;
}

void Gate::SetRightTransform(const Transform& transform)
{
	mGateRight->pos = transform.pos;
	mGateRight->scale = transform.scale;
	mGateRight->rot = transform.rot;
}

void Gate::SetNegotiationCollider(const SphereCollider collider)
{
	mNegotiationCollider = collider;
	mNegotiationCollider.isActive = true;
}

void Gate::SetCenterPos(const Vec3 pos)
{
	mCenterPos = pos;
}

void Gate::SetisOpening(const bool isOpening)
{
	mIsOpening = isOpening;
}

void Gate::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGateLeft->SetGraphicsPipeline(graphicsPipeline);
	mGateRight->SetGraphicsPipeline(graphicsPipeline);
}

CapsuleCollider Gate::GetLeftCollider()
{
	return mLeftCollider;
}

CapsuleCollider Gate::GetRightCollider()
{
	return mRightCollider;
}

CapsuleCollider Gate::GetCloseCollider()
{
	return mCloseCollider;
}

SphereCollider Gate::GetNegotiationCollider()
{
	return mNegotiationCollider;
}

Vec3 Gate::GetCenterPos()
{
	return mCenterPos;
}

Vec3 Gate::GetNegotitationPos()
{
	return mNegotitationPos;
}

bool Gate::GetisOpen()
{
	return mIsOpen;
}

