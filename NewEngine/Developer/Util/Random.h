#pragma once
#include "Vec3.h"
class Random
{
public:
	static void Initialize();
	static int Range(int min, int max);			// intŒ^‚Ì—”‚ğæ“¾
	static float RangeF(float min, float max);	// floatŒ^‚Ì—”‚ğæ“¾
	static bool RangeB(float rate); // Šm—¦‚Åtrue‚ğ•Ô‚·
};