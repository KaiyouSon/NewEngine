#pragma once
#include "Vec3.h"

class Random
{
public:
	static void Init();

	// intŒ^‚Ì—”‚ğæ“¾
	static int Range(const int min, const int max);

	// floatŒ^‚Ì—”‚ğæ“¾
	static float RangeF(const float min, const float max);

	// Vec2Œ^‚Ì—”‚ğæ“¾
	static Vec2 RangeVec2(const float min, const float max, const bool isSameValue = false);

	// Vec3Œ^‚Ì—”‚ğæ“¾
	static Vec3 RangeVec3(const float min, const float max, const bool isSameValue = false);

	// Šm—¦‚Åtrue‚ğ•Ô‚·
	static bool RangeB(const float rate);

	// ƒ‰ƒ“ƒ_ƒ€‚ÌŠp“x‚ğ•Ô‚·
	static float RangeAngle(const float min = 1.f, const float max = 360.f);

	// ƒ‰ƒ“ƒ_ƒ€‚Ìƒ‰ƒWƒAƒ“‚ğ•Ô‚·
	static float RangeRadian(const float min = 0.01745f, const float max = 6.28319f);

	// ƒ‰ƒ“ƒ_ƒ€‚Å‰~“à‚Ì“_‚ğ•Ô‚·
	static Vec2 RangeInCircle(const float minRadius, const float maxRadius);
};