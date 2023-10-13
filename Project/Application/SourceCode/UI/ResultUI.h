#pragma once
#include "NewEngine.h"
#include "IUI.h"

// リザルトUIのクラス
class ResultUI : public IUI
{
public:
	enum ResultType
	{
		EnemyFelledText,
		YouDiedText
	};

private:
	std::unique_ptr<Sprite> mBack;
	std::unique_ptr<Sprite> mText;
	std::unique_ptr<Sprite> mTextAfterImage;
	Transform mParent;
	Timer mTimer;
	bool mIsWait;
	Timer mWaitTimer;
	ResultType mResultType;

	bool mIsAfterImage;

	static bool sIsEnd;

private:
	// 敵を倒した時の処理
	void EnemyFelledUpdate();
	void EnemyFelledDraw();

	// プレイヤーが死んだ時の処理
	void YouDiedUpdate();
	void YouDiedDraw();

public:
	ResultUI();
	void Init();
	void Update();
	void Draw();

public:
	void SetResultType(const ResultType resultType);

	static bool GetisEnd();
};

