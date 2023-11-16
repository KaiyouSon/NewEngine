#include "stdafx.h"
#include "AirCollider.h"

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
