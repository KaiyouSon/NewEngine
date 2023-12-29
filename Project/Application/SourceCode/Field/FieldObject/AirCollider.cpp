#include "stdafx.h"
#include "AirCollider.h"

AirCollider::AirCollider()
{
	mType = FieldObjectType::AirCollider;
}

void AirCollider::Update()
{
	if (mCollider.isActive == true)
	{
		ColliderDrawer::GetInstance()->Bind(&mCollider);
	}
}

void AirCollider::SetCollider(const CapsuleCollider& collider)
{
	mCollider.isActive = true;
	mCollider = collider;
}

CapsuleCollider AirCollider::GetCollider()
{
	return mCollider;
}
