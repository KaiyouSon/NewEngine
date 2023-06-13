#pragma once
#include "Vec3.h"
#include <cstdint>

namespace Random
{
	void Init();

	// uint32_tŒ^‚Ì—”‚ğæ“¾
	int32_t Range(const int32_t min, const int32_t max);

	// floatŒ^‚Ì—”‚ğæ“¾
	float RangeF(const float min, const float max);

	// Vec2Œ^‚Ì—”‚ğæ“¾
	Vec2 RangeVec2(const float min, const float max, const bool isSameValue = false);

	// Vec3Œ^‚Ì—”‚ğæ“¾
	Vec3 RangeVec3(const float min, const float max, const bool isSameValue = false);

	// Šm—¦‚Åtrue‚ğ•Ô‚·
	bool RangeB(const float rate);

	// ƒ‰ƒ“ƒ_ƒ€‚ÌŠp“x‚ğ•Ô‚·
	float RangeAngle(const float min = 1.f, const float max = 360.f);

	// ƒ‰ƒ“ƒ_ƒ€‚Ìƒ‰ƒWƒAƒ“‚ğ•Ô‚·
	float RangeRadian(const float min = 0.01745f, const float max = 6.28319f);

	// ƒ‰ƒ“ƒ_ƒ€‚Å‰~“à‚Ì“_‚ğ•Ô‚·
	Vec2 RangeInCircle(const float minRadius, const float maxRadius);
}