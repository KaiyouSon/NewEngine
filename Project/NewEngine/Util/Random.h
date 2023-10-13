#pragma once
#include "Vec3.h"
#include <cstdint>

namespace Random
{
	void Init();

	// uint32_t蝙九・荵ｱ謨ｰ繧貞叙蠕・
	int32_t Range(const int32_t min, const int32_t max);

	// float蝙九・荵ｱ謨ｰ繧貞叙蠕・
	float RangeF(const float min, const float max);

	// Vec2蝙九・荵ｱ謨ｰ繧貞叙蠕・
	Vec2 RangeVec2(const float min, const float max, const bool isSameValue = false);

	// Vec3蝙九・荵ｱ謨ｰ繧貞叙蠕・
	Vec3 RangeVec3(const float min, const float max, const bool isSameValue = false);

	// 遒ｺ邇・〒true繧定ｿ斐☆
	bool RangeB(const float rate);

	// 繝ｩ繝ｳ繝繝縺ｮ隗貞ｺｦ繧定ｿ斐☆
	float RangeAngle(const float min = 1.f, const float max = 360.f);

	// 繝ｩ繝ｳ繝繝縺ｮ繝ｩ繧ｸ繧｢繝ｳ繧定ｿ斐☆
	float RangeRadian(const float min = 0.01745f, const float max = 6.28319f);

	// 繝ｩ繝ｳ繝繝縺ｧ蜀・・縺ｮ轤ｹ繧定ｿ斐☆
	Vec2 RangeInCircle(const float minRadius, const float maxRadius);
}
