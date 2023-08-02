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
	std::unique_ptr<Sprite> mBack;
	std::unique_ptr<Sprite> mText;
	std::unique_ptr<Sprite> mTextAfterImage;
	Transform mParent;
	bool mIsActive;
	Timer mTimer;
	bool mIsWait;
	Timer mWaitTimer;
	ResultType mResultType;

	bool mIsAfterImage;

	static bool sIsEnd;

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

	static bool GetisEnd();
};

