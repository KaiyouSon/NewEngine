#include "PlayerBody.h"
#include "Player.h"

PlayerBody::PlayerBody() :
	moveMotion_(std::make_unique<MoveMotion>()),
	backstepMotion_(std::make_unique<BackstepMotion>()),
	rollMotion_(std::make_unique<RollMotion>()),
	drinkMotion_(std::make_unique<DrinkMotion>())
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

void PlayerBody::Init()
{
	BaseInit();

	moveMotion_->Init(this);
	backstepMotion_->Init(this);
	rollMotion_->Init(this);
	drinkMotion_->Init(this);
}
void PlayerBody::DebugUpdate()
{
	static bool flag = false;
	if (Key::GetKeyDown(DIK_F5))
	{
		weapons_[1]->motion->Init(this);
		flag = true;
	}
	if (flag == true)
	{
		weapons_[1]->motion->WeakMotion(this);
		if (weapons_[1]->motion->GetisPlay() == false)
		{
			flag = false;
		}
	}

	if (Key::GetKeyDown(DIK_R))
	{
		flag = false;
		for (uint32_t i = 1; i < parts_.size(); i++)
		{
			parts_[i]->rot = 0;
		}
	}
}
void PlayerBody::Update()
{
	BaseUpdate();
}
void PlayerBody::DrawModel()
{
	BaseDrawModel();

	if (parent->state_ != Player::State::Drink)
	{
		DrawWeapon(WeaponPartID::Right);
	}
}
void PlayerBody::DrawDebugGui()
{
	BaseDrawDebugGui();
}

// モーション関連
void PlayerBody::IdleMotion()
{
	//for (uint32_t i = 1; i < parts_.size(); i++)
	//{
	//	parts_[i]->rot = 0;
	//}
}
void PlayerBody::JoggingMotion()
{
	moveMotion_->JoggingMotion(this);
}
void PlayerBody::RunMotion()
{
	moveMotion_->RunMotion(this);
}
void PlayerBody::AttackR1MotionUpdate()
{
	if (weapons_[(uint32_t)WeaponPartID::Right]->motion->GetisPlay() == true)
	{
		if (weapons_[(uint32_t)WeaponPartID::Right]->motion->GetisCanChangeMotion() == true)
		{
			// コンボできるため
			if (Pad::GetButtonDown(PadCode::ButtonR1))
			{
				weapons_[(uint32_t)WeaponPartID::Right]->motion->Init(this);
				weapons_[(uint32_t)WeaponPartID::Right]->motion->IncreComboCount();
			}
		}
	}

	weapons_[(uint32_t)WeaponPartID::Right]->motion->WeakMotion(this);
}
void PlayerBody::AttackR2MotionUpdate()
{
	if (weapons_[(uint32_t)WeaponPartID::Right]->motion->GetisPlay() == true)
	{
		//if (weapons_[0]->motion->GetisCanChangeMotion() == true)
		//{
		//	// コンボできるため
		//	if (Pad::GetButtonDown(PadCode::Butto))
		//	{
		//		weapons_[0]->motion->Init(this);
		//		weapons_[0]->motion->IncreComboCount();
		//	}
		//}
	}

	weapons_[(uint32_t)WeaponPartID::Right]->motion->HeavyMotion(this);
}
void PlayerBody::AttackBackMotionUpdate()
{
	weapons_[(uint32_t)WeaponPartID::Right]->motion->BackMotion(this);
}
void PlayerBody::AttackRollMotionUpdate()
{
	weapons_[(uint32_t)WeaponPartID::Right]->motion->RollMotion(this);
}
void PlayerBody::RollMotionUpdate()
{
	rollMotion_->Update(this);
}
void PlayerBody::BackstepMotionUpdate()
{
	backstepMotion_->Update(this);
}

void PlayerBody::ChangeMoveMotionInit()
{
	if (parent->state_ == Player::State::Jogging)
	{
		moveMotion_->JoggingInit(this);
	}
	else if (parent->state_ == Player::State::Run)
	{
		moveMotion_->RunInit(this);
	}
}
void PlayerBody::BackstepMotionInit()
{
	backstepMotion_->Init(this);
}
void PlayerBody::RollMotionInit()
{
	rollMotion_->Init(this);
}
void PlayerBody::AttackMotionInit(const WeaponPartID partID)
{
	weapons_[(uint32_t)partID]->motion->Init(this);
	weapons_[(uint32_t)partID]->motion->ResetComboCount();
}

bool PlayerBody::GetisPlayMoveMotion()
{
	return moveMotion_->GetisPlay();
}
bool PlayerBody::GetisPlayBackStepMotion()
{
	return backstepMotion_->GetisPlay();
}
bool PlayerBody::GetisPlayRollMotion()
{
	return rollMotion_->GetisPlay();
}
bool PlayerBody::GetisPlayAttackMotion(const WeaponPartID partID)
{
	return weapons_[(uint32_t)partID]->motion->GetisPlay();
}
bool PlayerBody::GetisAttackMotionCanChange(const WeaponPartID partID)
{
	return weapons_[(uint32_t)partID]->motion->GetisCanChangeMotion();
}
bool PlayerBody::GetisBackStepMotionCanChange()
{
	return backstepMotion_->GetisCanChangeMotion();
}
bool PlayerBody::GetisRollMotionCanChange()
{
	return rollMotion_->GetisCanChangeMotion();
}

Vec3 PlayerBody::GetWorldPos(const PartID partID)
{
	return parts_[(uint32_t)partID]->GetWorldPos();
}
Object3D* PlayerBody::GetPart(const PartID partID)
{
	return parts_[(uint32_t)partID].get();
}
Object3D* PlayerBody::GetWeaponPart(const WeaponPartID partID)
{
	if (weapons_[(uint32_t)partID] == nullptr)
	{
		return nullptr;
	}

	return weapons_[(uint32_t)partID]->weapon.get();
}
uint32_t PlayerBody::GetPartsSize()
{
	return (uint32_t)parts_.size();
}
uint32_t PlayerBody::GetWeaponPartsSize()
{
	return (uint32_t)weapons_.size();
}

