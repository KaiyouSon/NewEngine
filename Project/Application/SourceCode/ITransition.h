#pragma once

enum class TransitionType
{
	Respawn,
};

enum class TransitionStep
{
	None,
	In,
	Progress,
	Out,
	End,
};

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

