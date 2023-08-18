#pragma once
#include "NewEngine.h"
#include "ITransition.h"

template<typename T> class Singleton;

// ��g�����W�V�����S�����̃N���X�̂܂Ƃ߂�
class TransitionManager : public Singleton<TransitionManager>
{
private:
	std::unique_ptr<ITransition> mCurrnetTransition;

public:
	TransitionManager();
	void Update();
	void DrawFrontSprite();

	void Start(TransitionType type);
	void End();

	ITransition* GetCurrentTransition();
private:
	friend Singleton<TransitionManager>;
};

