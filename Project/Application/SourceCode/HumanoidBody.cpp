#include "HumanoidBody.h"
#include "Player.h"

HumanoidBody::HumanoidBody() :
	scale(1)
{
	for (uint32_t i = 0; i < parts_.size(); i++)
	{
		parts_[i] = std::make_unique<Object3D>();
		if (i > (uint32_t)PartID::Body)
		{
			parts_[i]->SetModel(ModelManager::GetModel("Limbs"));
		}
	}
	parts_[(uint32_t)PartID::Body]->SetModel(ModelManager::GetModel("Body"));
	parts_[(uint32_t)PartID::Head]->SetModel(ModelManager::GetModel("Head"));
}

void HumanoidBody::BaseInit()
{
	parts_[(uint32_t)PartID::Head]->pos = Vec3(0.f, 2.5f, 0.f);

	parts_[(uint32_t)PartID::RightArm]->pos = Vec3(1.5f, 1.5f, 0.f);
	parts_[(uint32_t)PartID::RightHand]->pos = Vec3(0.f, -1.5f, 0.f);
	parts_[(uint32_t)PartID::LeftArm]->pos = Vec3(-1.5f, 1.5f, 0.f);
	parts_[(uint32_t)PartID::LeftHand]->pos = Vec3(0.f, -1.5f, 0.f);

	parts_[(uint32_t)PartID::RightThigh]->pos = Vec3(0.5f, -1.5f, 0.f);
	parts_[(uint32_t)PartID::RightLeg]->pos = Vec3(0.f, -1.5f, 0.f);
	parts_[(uint32_t)PartID::LeftThigh]->pos = Vec3(-0.5f, -1.5f, 0.f);
	parts_[(uint32_t)PartID::LeftLeg]->pos = Vec3(0.f, -1.5f, 0.f);

	parts_[(uint32_t)PartID::RightArm]->color = Color::black;
	parts_[(uint32_t)PartID::RightHand]->color = Color::blue;
	parts_[(uint32_t)PartID::RightLeg]->color = Color::green;
	parts_[(uint32_t)PartID::LeftThigh]->color = Color::black;
	parts_[(uint32_t)PartID::LeftHand]->color = Color::green;
	parts_[(uint32_t)PartID::LeftLeg]->color = Color::blue;

}
void HumanoidBody::BaseUpdate()
{
	parts_[(uint32_t)PartID::Transform]->pos = pos;
	parts_[(uint32_t)PartID::Transform]->rot = rot;
	parts_[(uint32_t)PartID::Transform]->scale = scale;

	parts_[(uint32_t)PartID::Transform]->Update();
	Transform transform = parts_[(uint32_t)PartID::Transform]->GetTransform();

	// ëÃ
	parts_[(uint32_t)PartID::Body]->Update(&transform);
	Transform body = parts_[(uint32_t)PartID::Body]->GetTransform();

	// ì™
	parts_[(uint32_t)PartID::Head]->Update(&body);

	// ç∂éË
	parts_[(uint32_t)PartID::LeftArm]->Update(&body);
	Transform leftArm = parts_[(uint32_t)PartID::LeftArm]->GetTransform();
	parts_[(uint32_t)PartID::LeftHand]->Update(&leftArm);
	if (weapons_[0] != nullptr)
	{
		Transform leftHand = parts_[(uint32_t)PartID::LeftHand]->GetTransform();
		weapons_[0]->Update(&leftHand);
	}
	// ç∂ë´
	parts_[(uint32_t)PartID::LeftThigh]->Update(&body);
	Transform leftThigh = parts_[(uint32_t)PartID::LeftThigh]->GetTransform();
	parts_[(uint32_t)PartID::LeftLeg]->Update(&leftThigh);

	// âEéË
	parts_[(uint32_t)PartID::RightArm]->Update(&body);
	Transform rightArm = parts_[(uint32_t)PartID::RightArm]->GetTransform();
	parts_[(uint32_t)PartID::RightHand]->Update(&rightArm);
	if (weapons_[1] != nullptr)
	{
		Transform rightHand = parts_[(uint32_t)PartID::RightHand]->GetTransform();
		weapons_[1]->Update(&rightHand);
	}

	// âEë´
	parts_[(uint32_t)PartID::RightThigh]->Update(&body);
	Transform rightThigh = parts_[(uint32_t)PartID::RightThigh]->GetTransform();
	parts_[(uint32_t)PartID::RightLeg]->Update(&rightThigh);
}
void HumanoidBody::BaseDrawModel()
{
	for (uint32_t i = 1; i < parts_.size(); i++)
	{
		parts_[i]->Draw();
	}
}
void HumanoidBody::BaseDrawDebugGui()
{
	const float move = 0.05f;
	Vec3 angle;

	GuiManager::BeginWindow("HumanoidBody");

	GuiManager::DrawSlider3("Pos", pos, move);
	GuiManager::DrawSlider3("Rot", rot, move);
	GuiManager::DrawSlider3("Scale", scale, move);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::Head]->rot);
	GuiManager::DrawSlider3("Head Pos", parts_[(uint32_t)PartID::Head]->pos, move);
	GuiManager::DrawSlider3("Head Rot", angle, move);
	GuiManager::DrawSlider3("Head Scale", parts_[(uint32_t)PartID::Head]->scale, move);
	parts_[(uint32_t)PartID::Head]->rot = Radian(angle);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::Body]->rot);
	GuiManager::DrawSlider3("Body Pos", parts_[(uint32_t)PartID::Body]->pos, move);
	GuiManager::DrawSlider3("Body Rot", angle, move);
	GuiManager::DrawSlider3("Body Scale", parts_[(uint32_t)PartID::Body]->scale, move);
	parts_[(uint32_t)PartID::Body]->rot = Radian(angle);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::RightArm]->rot);
	GuiManager::DrawSlider3("RightArm Pos", parts_[(uint32_t)PartID::RightArm]->pos, move);
	GuiManager::DrawSlider3("RightArm Rot", angle, move);
	GuiManager::DrawSlider3("RightArm Scale", parts_[(uint32_t)PartID::RightArm]->scale, move);
	parts_[(uint32_t)PartID::RightArm]->rot = Radian(angle);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::RightHand]->rot);
	GuiManager::DrawSlider3("RightHand Pos", parts_[(uint32_t)PartID::RightHand]->pos, move);
	GuiManager::DrawSlider3("RightHand Rot", angle, move);
	GuiManager::DrawSlider3("RightHand Scale", parts_[(uint32_t)PartID::RightHand]->scale, move);
	parts_[(uint32_t)PartID::RightHand]->rot = Radian(angle);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::LeftArm]->rot);
	GuiManager::DrawSlider3("LeftArm Pos", parts_[(uint32_t)PartID::LeftArm]->pos, move);
	GuiManager::DrawSlider3("LeftArm Rot", angle, move);
	GuiManager::DrawSlider3("LeftArm Scale", parts_[(uint32_t)PartID::LeftArm]->scale, move);
	parts_[(uint32_t)PartID::LeftArm]->rot = Radian(angle);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::LeftHand]->rot);
	GuiManager::DrawSlider3("LeftHand Pos", parts_[(uint32_t)PartID::LeftHand]->pos, move);
	GuiManager::DrawSlider3("LeftHand Rot", angle, move);
	GuiManager::DrawSlider3("LeftHand Scale", parts_[(uint32_t)PartID::LeftHand]->scale, move);
	parts_[(uint32_t)PartID::LeftHand]->rot = Radian(angle);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::RightThigh]->rot);
	GuiManager::DrawSlider3("RightThigh Pos", parts_[(uint32_t)PartID::RightThigh]->pos, move);
	GuiManager::DrawSlider3("RightThigh Rot", angle, move);
	GuiManager::DrawSlider3("RightThigh Scale", parts_[(uint32_t)PartID::RightThigh]->scale, move);
	parts_[(uint32_t)PartID::RightThigh]->rot = Radian(angle);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::RightLeg]->rot);
	GuiManager::DrawSlider3("RightLeg Pos", parts_[(uint32_t)PartID::RightLeg]->pos, move);
	GuiManager::DrawSlider3("RightLeg Rot", angle, move);
	GuiManager::DrawSlider3("RightLeg Scale", parts_[(uint32_t)PartID::RightLeg]->scale, move);
	parts_[(uint32_t)PartID::RightLeg]->rot = Radian(angle);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::LeftThigh]->rot);
	GuiManager::DrawSlider3("LeftThigh Pos", parts_[(uint32_t)PartID::LeftThigh]->pos, move);
	GuiManager::DrawSlider3("LeftThigh Rot", angle, move);
	GuiManager::DrawSlider3("LeftThigh Scale", parts_[(uint32_t)PartID::LeftThigh]->scale, move);
	parts_[(uint32_t)PartID::LeftThigh]->rot = Radian(angle);
	GuiManager::DrawLine();

	angle = Angle(parts_[(uint32_t)PartID::LeftLeg]->rot);
	GuiManager::DrawSlider3("LeftLeg Pos", parts_[(uint32_t)PartID::LeftLeg]->pos, move);
	GuiManager::DrawSlider3("LeftLeg Rot", angle, move);
	GuiManager::DrawSlider3("LeftLeg Scale", parts_[(uint32_t)PartID::LeftLeg]->scale, move);
	parts_[(uint32_t)PartID::LeftLeg]->rot = Radian(angle);
	GuiManager::DrawLine();

	if (weapons_[0] != nullptr)
	{
		angle = Angle(weapons_[0]->weapon->rot);
		GuiManager::DrawSlider3("RightWeapon Pos", weapons_[0]->weapon->pos, move);
		GuiManager::DrawSlider3("RightWeapon Rot", angle, move);
		GuiManager::DrawSlider3("RightWeapon Scale", weapons_[0]->weapon->scale, move);
		weapons_[0]->weapon->rot = Radian(angle);
		GuiManager::DrawLine();
	}

	GuiManager::EndWindow();
}

void HumanoidBody::DrawWeapon(const WeaponPartID partID)
{
	if (weapons_[(uint32_t)partID] != nullptr)
	{
		weapons_[(uint32_t)partID]->DrawModel();
	}
}

// åªç›ÇÃäpìx
std::vector<Vec3> HumanoidBody::CalcCurRots()
{
	std::vector<Vec3> result;
	result.resize(parts_.size());

	for (uint32_t i = 0; i < parts_.size(); i++)
	{
		result[i] = parts_[i]->rot;
	}
	return result;
}
std::vector<Vec3> HumanoidBody::CalcCurWeaponPoses()
{
	std::vector<Vec3> result;
	result.resize(weapons_.size());

	for (uint32_t i = 0; i < weapons_.size(); i++)
	{
		if (weapons_[i] != nullptr)
		{
			result[i] = weapons_[i]->weapon->pos;
		}
	}
	return result;
}
std::vector<Vec3> HumanoidBody::CalcCurWeaponRots()
{
	std::vector<Vec3> result;
	result.resize(weapons_.size());

	for (uint32_t i = 0; i < weapons_.size(); i++)
	{
		if (weapons_[i] != nullptr)
		{
			result[i] = weapons_[i]->weapon->rot;
		}
	}
	return result;
}

void HumanoidBody::SetWeapon(Weapon* weapon, const WeaponPartID partID)
{
	weapons_[(uint32_t)partID] = weapon;
	weapons_[(uint32_t)partID]->weapon->pos.y = -1.5f;

	weapons_[(uint32_t)partID]->motion->Init(this);
	weapons_[(uint32_t)partID]->motion->ResetComboCount();
}

Vec3 HumanoidBody::GetWorldPos(const PartID partID)
{
	return parts_[(uint32_t)partID]->GetWorldPos();
}
Object3D* HumanoidBody::GetPart(const PartID partID)
{
	return parts_[(uint32_t)partID].get();
}
Object3D* HumanoidBody::GetWeaponPart(const WeaponPartID partID)
{
	if (weapons_[(uint32_t)partID] == nullptr)
	{
		return nullptr;
	}

	return weapons_[(uint32_t)partID]->weapon.get();
}
uint32_t HumanoidBody::GetPartsSize()
{
	return (uint32_t)parts_.size();
}
uint32_t HumanoidBody::GetWeaponPartsSize()
{
	return (uint32_t)weapons_.size();
}

