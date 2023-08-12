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

private:
	std::array<std::unique_ptr<Sprite>, 2> mTransition;
	std::array<ConstantBufferData::CRespawnTransition, 2> mTransitionData;
	std::array<ConstantBufferData::CUVParameter, 2> mUVParameteData;

public:
	RespawnTransition();
	void Generate();
	void Update();
	void DrawFrontSprite();
};

