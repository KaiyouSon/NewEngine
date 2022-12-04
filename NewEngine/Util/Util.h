#pragma once
#include "Color.h"
#include "FrameRate.h"
#include "Random.h"
#include "Singleton.h"
#include "MathUtil.h"
#include "Transform.h"
#include "Camera.h"
#include "SceneManager.h"
#include <memory>
#include <vector>
#include <string>
#include <list>

enum BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv
};

// ��r���đ傫������Ԃ�
float Max(float a, float b);

// ��r���ď���������Ԃ�
float Min(float a, float b);

// ������Ԃ��i -1, 0, 1 �j
int Sign(float a);
