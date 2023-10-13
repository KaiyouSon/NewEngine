#pragma once

// トランジションの種類
enum class TransitionType
{
	Respawn,
	Scene,
};

// トランジションのステップ
enum class TransitionStep
{
	None,
	In,
	Progress,
	Out,
	End,
};

// トランジションのインターフェース
class ITransition
{
protected:
	TransitionType mType;
	TransitionStep mStep;

public:
	virtual ~ITransition() {}
	virtual void Generate() = 0;
	virtual void Update() = 0;
	virtual void DrawFrontSprite() = 0;

public:
	TransitionType GetType();
	TransitionStep GetStep();
	void SetStep(const TransitionStep step);
};

