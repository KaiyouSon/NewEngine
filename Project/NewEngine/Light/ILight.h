#pragma once
#include "NewEngineEnum.h"

// ライトのインターフェース
class ILight
{
protected:
	LightType mLightType;

public:
	virtual ~ILight() {}
	LightType GetLightType();
};