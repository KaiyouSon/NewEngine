#include "HumanoidBody.h"

HumanoidBody::HumanoidBody() :
	frontVec(Vec3::front), scale(1)
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

	for (uint32_t i = 0; i < weapons_.size(); i++)
	{
		weapons_[i] = nullptr;
	}
}

void HumanoidBody::Init()
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

	joggingEase_.SetEaseTimer(20);
	joggingEase_.SetPowNum(2);
	runEase_.SetEaseTimer(15);
	runEase_.SetPowNum(2);
	isReverce_ = false;
}

void HumanoidBody::PrevUpdate()
{
	if (Key::GetKeyDown(DIK_SPACE))
	{
	}

	if (Key::GetKeyDown(DIK_R))
	{
		for (uint32_t i = 1; i < parts_.size(); i++)
		{
			parts_[i]->rot = 0;
		}
	}

	// コライダーの処理
	ColliderUpdate();
}

void HumanoidBody::PostUpdate()
{
	parts_[(uint32_t)PartID::Transform]->pos = pos;
	parts_[(uint32_t)PartID::Transform]->rot = rot;
	parts_[(uint32_t)PartID::Transform]->scale = scale;

	parts_[(uint32_t)PartID::Transform]->Update();
	Transform transform = parts_[(uint32_t)PartID::Transform]->GetTransform();

	parts_[(uint32_t)PartID::Body]->Update(&transform);
	Transform body = parts_[(uint32_t)PartID::Body]->GetTransform();

	parts_[(uint32_t)PartID::Head]->Update(&body);

	parts_[(uint32_t)PartID::RightArm]->Update(&body);
	Transform rightArm = parts_[(uint32_t)PartID::RightArm]->GetTransform();
	parts_[(uint32_t)PartID::RightHand]->Update(&rightArm);

	parts_[(uint32_t)PartID::LeftArm]->Update(&body);
	Transform leftArm = parts_[(uint32_t)PartID::LeftArm]->GetTransform();
	parts_[(uint32_t)PartID::LeftHand]->Update(&leftArm);

	parts_[(uint32_t)PartID::RightThigh]->Update(&body);
	Transform rightThigh = parts_[(uint32_t)PartID::RightThigh]->GetTransform();
	parts_[(uint32_t)PartID::RightLeg]->Update(&rightThigh);

	parts_[(uint32_t)PartID::LeftThigh]->Update(&body);
	Transform leftThigh = parts_[(uint32_t)PartID::LeftThigh]->GetTransform();
	parts_[(uint32_t)PartID::LeftLeg]->Update(&leftThigh);

	for (uint32_t i = 0; i < weapons_.size(); i++)
	{
		if (weapons_[i] != nullptr)
		{
			Transform rightHand = parts_[(uint32_t)PartID::RightHand]->GetTransform();
			weapons_[i]->Update(&rightHand);
		}
	}
}

void HumanoidBody::DrawModel()
{
	for (uint32_t i = 1; i < parts_.size(); i++)
	{
		parts_[i]->Draw();
	}

	for (uint32_t i = 0; i < weapons_.size(); i++)
	{
		if (weapons_[i] != nullptr)
		{
			weapons_[i]->DrawModel();
		}
	}
}

void HumanoidBody::DrawDebugGui()
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

void HumanoidBody::IdleMotion()
{
	//for (uint32_t i = 1; i < parts_.size(); i++)
	//{
	//	parts_[i]->rot = 0;
	//}
}

void HumanoidBody::JoggingMotion()
{
	parts_[(uint32_t)PartID::Body]->rot.x = Radian(5);
	parts_[(uint32_t)PartID::RightArm]->rot.z = Radian(5);
	parts_[(uint32_t)PartID::LeftArm]->rot.z = Radian(-5);

	if (isReverce_ == true)
	{
		// 体
		parts_[(uint32_t)PartID::Body]->rot.y = joggingEase_.InOut(Radian(10), Radian(-10));
		parts_[(uint32_t)PartID::Head]->rot.y = joggingEase_.InOut(Radian(-2), Radian(2));

		// 右腕
		parts_[(uint32_t)PartID::RightArm]->rot.x = joggingEase_.InOut(Radian(35), Radian(-15));
		parts_[(uint32_t)PartID::RightHand]->rot.x = joggingEase_.InOut(Radian(-70), Radian(-50));
		// 左腕
		parts_[(uint32_t)PartID::LeftArm]->rot.x = joggingEase_.InOut(Radian(-15), Radian(35));
		parts_[(uint32_t)PartID::LeftHand]->rot.x = joggingEase_.InOut(Radian(-50), Radian(-70));

		// 右足
		parts_[(uint32_t)PartID::RightThigh]->rot.x = joggingEase_.InOut(Radian(-50), Radian(30));
		parts_[(uint32_t)PartID::RightLeg]->rot.x = joggingEase_.InOut(Radian(30), Radian(10));
		// 左足
		parts_[(uint32_t)PartID::LeftThigh]->rot.x = joggingEase_.InOut(Radian(30), Radian(-50));
		parts_[(uint32_t)PartID::LeftLeg]->rot.x = joggingEase_.InOut(Radian(30), Radian(10));


		if (joggingEase_.GetisEnd() == true)
		{
			joggingEase_.Reset();
			isReverce_ = false;
		}
	}
	else if (isReverce_ == false)
	{
		// 体
		parts_[(uint32_t)PartID::Body]->rot.y = joggingEase_.InOut(Radian(-10), Radian(10));
		parts_[(uint32_t)PartID::Head]->rot.y = joggingEase_.InOut(Radian(2), Radian(-2));

		// 右腕
		parts_[(uint32_t)PartID::RightArm]->rot.x = joggingEase_.InOut(Radian(-15), Radian(35));
		parts_[(uint32_t)PartID::RightHand]->rot.x = joggingEase_.InOut(Radian(-50), Radian(-70));
		// 左腕
		parts_[(uint32_t)PartID::LeftArm]->rot.x = joggingEase_.InOut(Radian(35), Radian(-15));
		parts_[(uint32_t)PartID::LeftHand]->rot.x = joggingEase_.InOut(Radian(-70), Radian(-50));

		// 右足
		parts_[(uint32_t)PartID::RightThigh]->rot.x = joggingEase_.InOut(Radian(30), Radian(-50));
		parts_[(uint32_t)PartID::RightLeg]->rot.x = joggingEase_.InOut(Radian(10), Radian(30));
		// 左足
		parts_[(uint32_t)PartID::LeftThigh]->rot.x = joggingEase_.InOut(Radian(-50), Radian(30));
		parts_[(uint32_t)PartID::LeftLeg]->rot.x = joggingEase_.InOut(Radian(10), Radian(30));


		if (joggingEase_.GetisEnd() == true)
		{
			joggingEase_.Reset();
			isReverce_ = true;
		}
	}

	joggingEase_.Update();
}

void HumanoidBody::RunMotion()
{
	parts_[(uint32_t)PartID::Body]->rot.x = Radian(10);
	parts_[(uint32_t)PartID::RightArm]->rot.z = Radian(10);
	parts_[(uint32_t)PartID::LeftArm]->rot.z = Radian(-10);

	if (isReverce_ == true)
	{
		// 体
		parts_[(uint32_t)PartID::Body]->rot.y = runEase_.InOut(Radian(15), Radian(-15));
		parts_[(uint32_t)PartID::Head]->rot.y = runEase_.InOut(Radian(-10), Radian(10));

		// 右腕
		parts_[(uint32_t)PartID::RightArm]->rot.x = runEase_.InOut(Radian(60), Radian(-40));
		parts_[(uint32_t)PartID::RightHand]->rot.x = runEase_.InOut(Radian(-80), Radian(-50));

		// 左腕
		parts_[(uint32_t)PartID::LeftArm]->rot.x = runEase_.InOut(Radian(-40), Radian(60));
		parts_[(uint32_t)PartID::LeftHand]->rot.x = runEase_.InOut(Radian(-50), Radian(-80));

		// 右足
		parts_[(uint32_t)PartID::RightThigh]->rot.x = runEase_.InOut(Radian(-70), Radian(40));
		parts_[(uint32_t)PartID::RightLeg]->rot.x = runEase_.InOut(Radian(15), Radian(30));
		// 左足
		parts_[(uint32_t)PartID::LeftThigh]->rot.x = runEase_.InOut(Radian(40), Radian(-70));
		parts_[(uint32_t)PartID::LeftLeg]->rot.x = runEase_.InOut(Radian(30), Radian(15));


		if (runEase_.GetisEnd() == true)
		{
			runEase_.Reset();
			isReverce_ = false;
		}
	}
	else if (isReverce_ == false)
	{
		// 体
		parts_[(uint32_t)PartID::Body]->rot.y = runEase_.InOut(Radian(-15), Radian(15));
		parts_[(uint32_t)PartID::Head]->rot.y = runEase_.InOut(Radian(10), Radian(-10));

		// 右腕
		parts_[(uint32_t)PartID::RightArm]->rot.x = runEase_.InOut(Radian(-40), Radian(60));
		parts_[(uint32_t)PartID::RightHand]->rot.x = runEase_.InOut(Radian(-50), Radian(-80));

		// 左腕
		parts_[(uint32_t)PartID::LeftArm]->rot.x = runEase_.InOut(Radian(60), Radian(-40));
		parts_[(uint32_t)PartID::LeftHand]->rot.x = runEase_.InOut(Radian(-80), Radian(-50));


		// 右足
		parts_[(uint32_t)PartID::RightThigh]->rot.x = runEase_.InOut(Radian(40), Radian(-70));
		parts_[(uint32_t)PartID::RightLeg]->rot.x = runEase_.InOut(Radian(30), Radian(15));
		// 左足
		parts_[(uint32_t)PartID::LeftThigh]->rot.x = runEase_.InOut(Radian(-70), Radian(40));
		parts_[(uint32_t)PartID::LeftLeg]->rot.x = runEase_.InOut(Radian(15), Radian(30));


		if (runEase_.GetisEnd() == true)
		{
			runEase_.Reset();
			isReverce_ = true;
		}
	}

	runEase_.Update();

	//parts_[(uint32_t)PartID::RightHand]->rot.x = Radian(-50);
	//parts_[(uint32_t)PartID::LeftHand]->rot.x = Radian(-50);
}

void HumanoidBody::AttackMotion()
{
	if (weapons_[0]->motion->GetisPlay() == false)
	{
		weapons_[0]->motion->SetisPlay(true);
	}
	else
	{
		if (weapons_[0]->motion->GetisCanCombo() == true)
		{
			// コンボできるため
			if (Pad::GetButton(PadCode::ButtonR1))
			{
				weapons_[0]->motion->Init();
				weapons_[0]->motion->SetisPlay(true);
				weapons_[0]->motion->IncreComboCount();
			}
		}
	}

	weapons_[0]->motion->AttackMotion(this);
}

void HumanoidBody::ColliderUpdate()
{
	bodyCollider_.startPos = pos - Vec3(0.f, 2.5f, 0.f);
	bodyCollider_.endPos = pos + Vec3(0.f, 2.5f, 0.f);
	bodyCollider_.radius = 2.5f;
}

void HumanoidBody::CalcFrontVec()
{
	// カメラの前ベクトル
	Vec3 cameForward = pos - Camera::current.pos;
	cameForward.y = 0.f;

	// カメラの右ベクトル
	Vec3 cameRight = Vec3::Cross(cameForward, Vec3::up);

	Vec3 stick =
	{
		Pad::GetStick(PadCode::LeftStick, 300).x,
		0,
		Pad::GetStick(PadCode::LeftStick, 300).y,
	};
	if (stick != 0)
	{
		Vec3 stickMoveVec = 0;

		stickMoveVec.x = -stick.Norm().x;
		stickMoveVec.z = -stick.Norm().z;

		frontVec = cameForward * stickMoveVec.z + cameRight * stickMoveVec.x;
	}
}

void HumanoidBody::SetWeapon(Weapon* weapon, const uint32_t index)
{
	weapons_[index] = weapon;
	weapons_[index]->weapon->pos.y = -1.5f;
	weapons_[index]->weapon->rot.x = Radian(90);
}

bool HumanoidBody::GetisPlayAttackMotion(const uint32_t index)
{
	return weapons_[index]->motion->GetisPlay();
}

CapsuleCollider HumanoidBody::GetBodyCollider()
{
	return bodyCollider_;
}

Vec3 HumanoidBody::GetWorldPos(const PartID partID)
{
	return parts_[(uint32_t)partID]->GetWorldPos();
}

Object3D* HumanoidBody::GetPart(const PartID partID)
{
	return parts_[(uint32_t)partID].get();
}

uint32_t HumanoidBody::GetPartsSize()
{
	return (uint32_t)parts_.size();
}
