#include "TransitionManager.h"
#include "RespawnTransition.h"
#include "SceneTransition.h"

TransitionManager::TransitionManager() :
	mCurrnetTransition(nullptr)
{
}

void TransitionManager::Update()
{
	if (mCurrnetTransition == nullptr)
	{
		return;
	}
	mCurrnetTransition->Update();

	if (mCurrnetTransition->GetStep() == TransitionStep::End)
	{
		mCurrnetTransition.release();
		mCurrnetTransition = nullptr;
	}
}

void TransitionManager::DrawFrontSprite()
{
	if (mCurrnetTransition == nullptr)
	{
		return;
	}
	mCurrnetTransition->DrawFrontSprite();
}

void TransitionManager::Start(TransitionType type)
{
	if (mCurrnetTransition != nullptr)
	{
		return;
	}

	switch (type)
	{
	case TransitionType::Respawn:
		mCurrnetTransition = std::make_unique<RespawnTransition>();
		mCurrnetTransition->Generate();
		mCurrnetTransition->SetStep(TransitionStep::In);
		break;

	case TransitionType::Scene:
		mCurrnetTransition = std::make_unique<SceneTransition>();
		mCurrnetTransition->Generate();
		mCurrnetTransition->SetStep(TransitionStep::In);
		break;

	default:
		break;
	}
}

void TransitionManager::End()
{
	mCurrnetTransition->SetStep(TransitionStep::Out);
}

ITransition* TransitionManager::GetCurrentTransition()
{
	return mCurrnetTransition.get();
}
