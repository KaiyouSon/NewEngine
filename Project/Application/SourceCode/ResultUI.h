#pragma once
#include "NewEngine.h"

class ResultUI
{
public:
	enum ResultType
	{
		EnemyFelledStr,
		YouDiedStr
	};

private:
	std::unique_ptr<Sprite> back_;
	std::unique_ptr<Sprite> text_;
	std::unique_ptr<Sprite> textAfterImage_;
	Transform parent_;
	bool isActive_;
	Timer timer_;
	bool isWait_;
	Timer waitTimer_;
	ResultType resultType_;

	bool isAfterImage_;

private:
	void EnemyFelledUpdate();
	void YouDiedUpdate();

	void EnemyFelledDraw();
	void YouDiedDraw();

public:
	ResultUI();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetisActive(const bool isActive);
	void SetResultType(const ResultType resultType);
};

