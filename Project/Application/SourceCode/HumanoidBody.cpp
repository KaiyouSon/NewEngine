#include "HumanoidBody.h"
#include "Player.h"

HumanoidBody::HumanoidBody() :
	scale(1)
{
	for (uint32_t i = 0; i < mParts.size(); i++)
	{
		mParts[i] = std::make_unique<Object3D>();
		if (i > (uint32_t)PartID::Body)
		{
			mParts[i]->SetModel(ModelManager::GetModel("Limbs"));
		}
	}
	mParts[(uint32_t)PartID::Body]->SetModel(ModelManager::GetModel("Body"));
	mParts[(uint32_t)PartID::Head]->SetModel(ModelManager::GetModel("Head"));
}

void HumanoidBody::BaseInit()
{
	mParts[(uint32_t)PartID::Head]->pos = Vec3(0.f, 2.5f, 0.f);

	mParts[(uint32_t)PartID::RightArm]->pos = Vec3(1.5f, 1.5f, 0.f);
	mParts[(uint32_t)PartID::RightHand]->pos = Vec3(0.f, -1.5f, 0.f);
	mParts[(uint32_t)PartID::LeftArm]->pos = Vec3(-1.5f, 1.5f, 0.f);
	mParts[(uint32_t)PartID::LeftHand]->pos = Vec3(0.f, -1.5f, 0.f);

	mParts[(uint32_t)PartID::RightThigh]->pos = Vec3(0.5f, -1.5f, 0.f);
	mParts[(uint32_t)PartID::RightLeg]->pos = Vec3(0.f, -1.5f, 0.f);
	mParts[(uint32_t)PartID::LeftThigh]->pos = Vec3(-0.5f, -1.5f, 0.f);
	mParts[(uint32_t)PartID::LeftLeg]->pos = Vec3(0.f, -1.5f, 0.f);

	mParts[(uint32_t)PartID::RightArm]->color = Color::black;
	mParts[(uint32_t)PartID::RightHand]->color = Color::blue;
	mParts[(uint32_t)PartID::RightLeg]->color = Color::green;
	mParts[(uint32_t)PartID::LeftThigh]->color = Color::black;
	mParts[(uint32_t)PartID::LeftHand]->color = Color::green;
	mParts[(uint32_t)PartID::LeftLeg]->color = Color::blue;
}
void HumanoidBody::BaseUpdate()
{
	mParts[(uint32_t)PartID::Transform]->pos = pos;
	mParts[(uint32_t)PartID::Transform]->rot = rot;
	mParts[(uint32_t)PartID::Transform]->scale = scale;

	mParts[(uint32_t)PartID::Transform]->Update();
	Transform transform = mParts[(uint32_t)PartID::Transform]->GetTransform();

	// ëÃ
	mParts[(uint32_t)PartID::Body]->Update(&transform);
	Transform body = mParts[(uint32_t)PartID::Body]->GetTransform();

	// ì™
	mParts[(uint32_t)PartID::Head]->Update(&body);

	// ç∂éË
	mParts[(uint32_t)PartID::LeftArm]->Update(&body);
	Transform leftArm = mParts[(uint32_t)PartID::LeftArm]->GetTransform();
	mParts[(uint32_t)PartID::LeftHand]->Update(&leftArm);
	if (mWeapons[0] != nullptr)
	{
		Transform leftHand = mParts[(uint32_t)PartID::LeftHand]->GetTransform();
		mWeapons[0]->Update(&leftHand);
	}
	// ç∂ë´
	mParts[(uint32_t)PartID::LeftThigh]->Update(&body);
	Transform leftThigh = mParts[(uint32_t)PartID::LeftThigh]->GetTransform();
	mParts[(uint32_t)PartID::LeftLeg]->Update(&leftThigh);

	// âEéË
	mParts[(uint32_t)PartID::RightArm]->Update(&body);
	Transform rightArm = mParts[(uint32_t)PartID::RightArm]->GetTransform();
	mParts[(uint32_t)PartID::RightHand]->Update(&rightArm);
	if (mWeapons[1] != nullptr)
	{
		Transform rightHand = mParts[(uint32_t)PartID::RightHand]->GetTransform();
		mWeapons[1]->Update(&rightHand);
	}

	// âEë´
	mParts[(uint32_t)PartID::RightThigh]->Update(&body);
	Transform rightThigh = mParts[(uint32_t)PartID::RightThigh]->GetTransform();
	mParts[(uint32_t)PartID::RightLeg]->Update(&rightThigh);
}
void HumanoidBody::BaseDrawModel()
{
	for (uint32_t i = 1; i < mParts.size(); i++)
	{
		mParts[i]->Draw();
	}
}
void HumanoidBody::BaseDrawDebugGui()
{
	const float move = 0.05f;
	Vec3 angle;

	Gui::BeginWindow("HumanoidBody");

	Gui::DrawSlider3("Pos", pos, move);
	Gui::DrawSlider3("Rot", rot, move);
	Gui::DrawSlider3("Scale", scale, move);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::Head]->rot);
	Gui::DrawSlider3("Head Pos", mParts[(uint32_t)PartID::Head]->pos, move);
	Gui::DrawSlider3("Head Rot", angle, move);
	Gui::DrawSlider3("Head Scale", mParts[(uint32_t)PartID::Head]->scale, move);
	mParts[(uint32_t)PartID::Head]->rot = Radian(angle);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::Body]->rot);
	Gui::DrawSlider3("Body Pos", mParts[(uint32_t)PartID::Body]->pos, move);
	Gui::DrawSlider3("Body Rot", angle, move);
	Gui::DrawSlider3("Body Scale", mParts[(uint32_t)PartID::Body]->scale, move);
	mParts[(uint32_t)PartID::Body]->rot = Radian(angle);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::RightArm]->rot);
	Gui::DrawSlider3("RightArm Pos", mParts[(uint32_t)PartID::RightArm]->pos, move);
	Gui::DrawSlider3("RightArm Rot", angle, move);
	Gui::DrawSlider3("RightArm Scale", mParts[(uint32_t)PartID::RightArm]->scale, move);
	mParts[(uint32_t)PartID::RightArm]->rot = Radian(angle);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::RightHand]->rot);
	Gui::DrawSlider3("RightHand Pos", mParts[(uint32_t)PartID::RightHand]->pos, move);
	Gui::DrawSlider3("RightHand Rot", angle, move);
	Gui::DrawSlider3("RightHand Scale", mParts[(uint32_t)PartID::RightHand]->scale, move);
	mParts[(uint32_t)PartID::RightHand]->rot = Radian(angle);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::LeftArm]->rot);
	Gui::DrawSlider3("LeftArm Pos", mParts[(uint32_t)PartID::LeftArm]->pos, move);
	Gui::DrawSlider3("LeftArm Rot", angle, move);
	Gui::DrawSlider3("LeftArm Scale", mParts[(uint32_t)PartID::LeftArm]->scale, move);
	mParts[(uint32_t)PartID::LeftArm]->rot = Radian(angle);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::LeftHand]->rot);
	Gui::DrawSlider3("LeftHand Pos", mParts[(uint32_t)PartID::LeftHand]->pos, move);
	Gui::DrawSlider3("LeftHand Rot", angle, move);
	Gui::DrawSlider3("LeftHand Scale", mParts[(uint32_t)PartID::LeftHand]->scale, move);
	mParts[(uint32_t)PartID::LeftHand]->rot = Radian(angle);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::RightThigh]->rot);
	Gui::DrawSlider3("RightThigh Pos", mParts[(uint32_t)PartID::RightThigh]->pos, move);
	Gui::DrawSlider3("RightThigh Rot", angle, move);
	Gui::DrawSlider3("RightThigh Scale", mParts[(uint32_t)PartID::RightThigh]->scale, move);
	mParts[(uint32_t)PartID::RightThigh]->rot = Radian(angle);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::RightLeg]->rot);
	Gui::DrawSlider3("RightLeg Pos", mParts[(uint32_t)PartID::RightLeg]->pos, move);
	Gui::DrawSlider3("RightLeg Rot", angle, move);
	Gui::DrawSlider3("RightLeg Scale", mParts[(uint32_t)PartID::RightLeg]->scale, move);
	mParts[(uint32_t)PartID::RightLeg]->rot = Radian(angle);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::LeftThigh]->rot);
	Gui::DrawSlider3("LeftThigh Pos", mParts[(uint32_t)PartID::LeftThigh]->pos, move);
	Gui::DrawSlider3("LeftThigh Rot", angle, move);
	Gui::DrawSlider3("LeftThigh Scale", mParts[(uint32_t)PartID::LeftThigh]->scale, move);
	mParts[(uint32_t)PartID::LeftThigh]->rot = Radian(angle);
	Gui::DrawLine();

	angle = Angle(mParts[(uint32_t)PartID::LeftLeg]->rot);
	Gui::DrawSlider3("LeftLeg Pos", mParts[(uint32_t)PartID::LeftLeg]->pos, move);
	Gui::DrawSlider3("LeftLeg Rot", angle, move);
	Gui::DrawSlider3("LeftLeg Scale", mParts[(uint32_t)PartID::LeftLeg]->scale, move);
	mParts[(uint32_t)PartID::LeftLeg]->rot = Radian(angle);
	Gui::DrawLine();

	if (mWeapons[0] != nullptr)
	{
		angle = Angle(mWeapons[0]->weapon->rot);
		Gui::DrawSlider3("RightWeapon Pos", mWeapons[0]->weapon->pos, move);
		Gui::DrawSlider3("RightWeapon Rot", angle, move);
		Gui::DrawSlider3("RightWeapon Scale", mWeapons[0]->weapon->scale, move);
		mWeapons[0]->weapon->rot = Radian(angle);
		Gui::DrawLine();
	}

	Gui::EndWindow();
}

void HumanoidBody::DrawWeapon(const WeaponPartID partID)
{
	if (mWeapons[(uint32_t)partID] != nullptr)
	{
		mWeapons[(uint32_t)partID]->DrawModel();
	}
}

// åªç›ÇÃäpìx
std::vector<Vec3> HumanoidBody::CalcCurRots()
{
	std::vector<Vec3> result;
	result.resize(mParts.size());

	for (uint32_t i = 0; i < mParts.size(); i++)
	{
		result[i] = mParts[i]->rot;
	}
	return result;
}
std::vector<Vec3> HumanoidBody::CalcCurWeaponPoses()
{
	std::vector<Vec3> result;
	result.resize(mWeapons.size());

	for (uint32_t i = 0; i < mWeapons.size(); i++)
	{
		if (mWeapons[i] != nullptr)
		{
			result[i] = mWeapons[i]->weapon->pos;
		}
	}
	return result;
}
std::vector<Vec3> HumanoidBody::CalcCurWeaponRots()
{
	std::vector<Vec3> result;
	result.resize(mWeapons.size());

	for (uint32_t i = 0; i < mWeapons.size(); i++)
	{
		if (mWeapons[i] != nullptr)
		{
			result[i] = mWeapons[i]->weapon->rot;
		}
	}
	return result;
}

void HumanoidBody::SetWeapon(Weapon* weapon, const WeaponPartID partID)
{
	mWeapons[(uint32_t)partID] = weapon;
	mWeapons[(uint32_t)partID]->weapon->pos.y = -1.5f;

	mWeapons[(uint32_t)partID]->motion->Init(this);
	mWeapons[(uint32_t)partID]->motion->ResetComboCount();
}

Vec3 HumanoidBody::GetWorldPos(const PartID partID)
{
	return mParts[(uint32_t)partID]->GetWorldPos();
}
Object3D* HumanoidBody::GetPart(const PartID partID)
{
	return mParts[(uint32_t)partID].get();
}
Object3D* HumanoidBody::GetWeaponPart(const WeaponPartID partID)
{
	if (mWeapons[(uint32_t)partID] == nullptr)
	{
		return nullptr;
	}

	return mWeapons[(uint32_t)partID]->weapon.get();
}
uint32_t HumanoidBody::GetPartsSize()
{
	return (uint32_t)mParts.size();
}
uint32_t HumanoidBody::GetWeaponPartsSize()
{
	return (uint32_t)mWeapons.size();
}

