#pragma once
#include "NewEngine.h"

class RespawnTransition
{
private:
	enum Layer
	{
		Back,
		Front,
	};

public:
	enum Step
	{
		None,
		In,
		Progress,
		Out,
		End,
	};

private:
	std::array<std::unique_ptr<Sprite>, 2> mTransition;
	std::array<ConstantBufferData::CRespawnTransition, 2> mTransitionData;
	std::array<ConstantBufferData::CUVParameter, 2> mUVParameteData;

	Step mStep;

public:
	RespawnTransition();
	void Generate();
	void Update();
	void DrawFrontSprite();

public:
	Step GetStep();
	void SetStep(const Step step);
};

