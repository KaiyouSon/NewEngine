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

enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
	Screen
};

// 比較して大きい方を返す
float Max(const float& a, const float& b);

// 比較して小さい方を返す
float Min(const float& a, const float& b);

// 符号を返す（ -1, 0, 1 ）
int Sign(const float& a);

// 値をMinとMaxの間に制限する関数
float Clamp(const float& value, const float& min = 0, const float& max = 1);

// 値が収束する関数
float Convergence(const float& value, const float& speed, const float& origin = 0);

// 桁数を取得
int GetDight(const int& value);

// ワールド座標をスクリーン座標に変換する
Vec2 WorldToScreen(const Vec3& worldPos);