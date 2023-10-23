#pragma once
#include "NewEngine.h"
#include "IUI.h"

class OperationUI : public IUI
{
private:
	struct Pair
	{
		std::unique_ptr<Sprite> mBack;
		std::unique_ptr<Sprite> mText;
		Pair();
	};

private:
	enum OperationType
	{
		Camera,
		WeekAttack,
		HeavyAttack,
		BackStep,
		UseItem,
		Size,
	};

private:
	std::vector<std::unique_ptr<Pair>> mPair;

public:
	OperationUI();
	void Init() override;
	void Update() override;
	void Draw() override;
};