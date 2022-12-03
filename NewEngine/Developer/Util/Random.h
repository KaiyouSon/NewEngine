#pragma once
#include "Vec3.h"
class Random
{
public:
	static void Initialize();
	static int Range(int min, int max);			// int型の乱数を取得
	static float RangeF(float min, float max);	// float型の乱数を取得
	static bool RangeB(float rate); // 確率でtrueを返す
};