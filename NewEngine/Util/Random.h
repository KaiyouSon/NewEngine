#pragma once
#include "Vec3.h"
class Random
{
public:
	static void Initialize();
	static int Range(const int& min, const int& max);			// intŒ^‚Ì—”‚ğæ“¾
	static float RangeF(const float& min, const float& max);	// floatŒ^‚Ì—”‚ğæ“¾
	static bool RangeB(const float& rate);						// Šm—¦‚Åtrue‚ğ•Ô‚·
};