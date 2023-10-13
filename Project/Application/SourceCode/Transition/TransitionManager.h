#pragma once
#include "NewEngine.h"
#include "ITransition.h"

// 前方宣言
template<typename T> class Singleton;

// トランジションを管理するクラス
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

