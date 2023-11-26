#include "PlayerBody.h"
#include "Player.h"
#include "GaugeType.h"

PlayerBody::PlayerBody() :
	mMoveMotion(std::make_unique<MoveMotion>()),
	mBackstepMotion(std::make_unique<BackstepMotion>()),
	mRollMotion(std::make_unique<RollMotion>()),
	mDrinkMotion(std::make_unique<DrinkMotion>()),
	mOpenGateMotion(std::make_unique<OpenGateMotion>())
{
	for (uint32_t i = 0; i < mParts.size(); i++)
	{
		mParts[i] = std::make_unique<Object3D>();
		if (i > (uint32_t)PartID::Body)
		{
			mParts[i]->SetModel(ModelManager::GetModel("Limbs"));
		}
		mParts[i]->SetisShadow(false, true);
	}
	mParts[(uint32_t)PartID::Body]->SetModel(ModelManager::GetModel("Body"));
	mParts[(uint32_t)PartID::Head]->SetModel(ModelManager::GetModel("Head"));

	mParts[(uint32_t)PartID::LeftArm]->color = Color::red;
	mParts[(uint32_t)PartID::RightArm]->color = Color::red;
	mParts[(uint32_t)PartID::LeftThigh]->color = Color::red;
	mParts[(uint32_t)PartID::RightThigh]->color = Color::red;

	mParts[(uint32_t)PartID::LeftHand]->scale = Vec3(0.99f, 1.0f, 0.99f);
	mParts[(uint32_t)PartID::RightHand]->scale = Vec3(0.99f, 1.0f, 0.99f);
	mParts[(uint32_t)PartID::LeftLeg]->scale = Vec3(0.99f, 1.0f, 0.99f);
	mParts[(uint32_t)PartID::RightLeg]->scale = Vec3(0.99f, 1.0f, 0.99f);
}

void PlayerBody::Init()
{
	BaseInit();

	for (uint32_t i = 1; i < mParts.size(); i++)
	{
		mParts[i]->isUseDissolve = true;
		mParts[i]->colorPower = 5;
		mParts[i]->dissolveColor = Color(255, 30, 0, 255);
	}

	mMoveMotion->Init(this);
	mBackstepMotion->Init(this);
	mRollMotion->Init(this);
	mDrinkMotion->Init(this);
	mOpenGateMotion->Init(this);
}
void PlayerBody::DebugUpdate()
{
	static bool flag = false;
	if (Key::GetKeyDown(DIK_F4))
	{
		mWeapons[1]->motion->Init(this);
		flag = true;
	}
	if (flag == true)
	{
		mWeapons[1]->motion->WeakMotion(this);
		if (mWeapons[1]->motion->GetisPlay() == false)
		{
			flag = false;
		}
	}

	if (Key::GetKeyDown(DIK_R))
	{
		flag = false;
		for (uint32_t i = 1; i < mParts.size(); i++)
		{
			mParts[i]->rot = 0;
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

	bool isDrinkMotion = parent->mState == Player::State::Drink;
	bool isOpenGateMotion = parent->mState == Player::State::OpenGate;
	bool isMovieEvent = parent->mMovieEvent->GetisPlaying();

	if (isDrinkMotion == true ||
		isOpenGateMotion == true ||
		isMovieEvent == true)
	{
		return;
	}
	DrawWeapon(WeaponPartID::Right);
}
void PlayerBody::DrawDebugGui()
{
	BaseDrawDebugGui();
}

void PlayerBody::IdleMotion()
{
	//for (uint32_t i = 1; i < mParts.size(); i++)
	//{
	//	mParts[i]->rot = 0;
	//}
}
void PlayerBody::JoggingMotion()
{
	mMoveMotion->JoggingMotion(this);
}
void PlayerBody::RunMotion()
{
	mMoveMotion->RunMotion(this);
}
void PlayerBody::AttackR1MotionUpdate()
{
	if (mWeapons[(uint32_t)WeaponPartID::Right]->motion->GetisPlay() == true)
	{
		if (mWeapons[(uint32_t)WeaponPartID::Right]->motion->GetisCanChangeMotion() == true)
		{
			if (Pad::GetButtonDown(PadCode::ButtonR1))
			{
				if (parent->mGaugePrames[(uint32_t)GaugeType::Stamina].value - 20 >= 0)
				{
					parent->mGaugePrames[(uint32_t)GaugeType::Stamina].value -= 20;
					mWeapons[(uint32_t)WeaponPartID::Right]->motion->Init(this);
					mWeapons[(uint32_t)WeaponPartID::Right]->motion->IncreComboCount();
				}
			}
		}
	}

	mWeapons[(uint32_t)WeaponPartID::Right]->motion->WeakMotion(this);
}
void PlayerBody::AttackR2MotionUpdate()
{
	mWeapons[(uint32_t)WeaponPartID::Right]->motion->HeavyMotion(this);
}
void PlayerBody::AttackBackMotionUpdate()
{
	mWeapons[(uint32_t)WeaponPartID::Right]->motion->BackMotion(this);
}
void PlayerBody::AttackRollMotionUpdate()
{
	mWeapons[(uint32_t)WeaponPartID::Right]->motion->RollMotion(this);
}
void PlayerBody::RollMotionUpdate()
{
	mRollMotion->Update(this);
}
void PlayerBody::BackstepMotionUpdate()
{
	mBackstepMotion->Update(this);
}

void PlayerBody::ChangeMoveMotionInit()
{
	if (parent->mState == Player::State::Jogging)
	{
		mMoveMotion->JoggingInit(this);
	}
	else if (parent->mState == Player::State::Run)
	{
		mMoveMotion->RunInit(this);
	}
}
void PlayerBody::BackstepMotionInit()
{
	mBackstepMotion->Init(this);
}
void PlayerBody::RollMotionInit()
{
	mRollMotion->Init(this);
}
void PlayerBody::AttackMotionInit(const WeaponPartID partID)
{
	mWeapons[(uint32_t)partID]->motion->Init(this);
	mWeapons[(uint32_t)partID]->motion->ResetComboCount();
}

bool PlayerBody::GetisPlayMoveMotion()
{
	return mMoveMotion->GetisPlay();
}
bool PlayerBody::GetisPlayBackStepMotion()
{
	return mBackstepMotion->GetisPlay();
}
bool PlayerBody::GetisPlayRollMotion()
{
	return mRollMotion->GetisPlay();
}
bool PlayerBody::GetisPlayAttackMotion(const WeaponPartID partID)
{
	return mWeapons[(uint32_t)partID]->motion->GetisPlay();
}
bool PlayerBody::GetisAttackMotionCanChange(const WeaponPartID partID)
{
	return mWeapons[(uint32_t)partID]->motion->GetisCanChangeMotion();
}
bool PlayerBody::GetisBackStepMotionCanChange()
{
	return mBackstepMotion->GetisCanChangeMotion();
}
bool PlayerBody::GetisRollMotionCanChange()
{
	return mRollMotion->GetisCanChangeMotion();
}

Vec3 PlayerBody::GetWorldPos(const PartID partID)
{
	return mParts[(uint32_t)partID]->GetWorldPos();
}
Object3D* PlayerBody::GetPart(const PartID partID)
{
	return mParts[(uint32_t)partID].get();
}
Object3D* PlayerBody::GetWeaponPart(const WeaponPartID partID)
{
	if (mWeapons[(uint32_t)partID] == nullptr)
	{
		return nullptr;
	}

	return mWeapons[(uint32_t)partID]->weapon.get();
}
uint32_t PlayerBody::GetPartsSize()
{
	return (uint32_t)mParts.size();
}
uint32_t PlayerBody::GetWeaponPartsSize()
{
	return (uint32_t)mWeapons.size();
}
