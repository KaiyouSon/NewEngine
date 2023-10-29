#pragma once
#include "NewEngineEnum.h"

// ライトのインターフェース
class ILight
{
protected:
	LightType mType;

public:
	virtual ~ILight() {}
	LightType GetLightType();
};