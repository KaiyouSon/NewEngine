#pragma once
#include "Vec3.h"

struct Vec3;

struct Vec2
{
	// 螟画焚
	float x; // 繝吶け繝医Ν縺ｮ x 謌仙・
	float y; // 繝吶け繝医Ν縺ｮ y 謌仙・

	// static螟画焚
	const static Vec2 left;		// Vector2(-1, 0) 縺ｨ蜷後§諢丞袖
	const static Vec2 right;	// Vector2(1, 0) 縺ｨ蜷後§諢丞袖
	const static Vec2 up;		// Vector2(0, 1) 縺ｨ蜷後§諢丞袖
	const static Vec2 down;		// Vector2(0, -1) 縺ｨ蜷後§諢丞袖
	const static Vec2 one;		// Vector2(1, 1) 縺ｨ蜷後§諢丞袖
	const static Vec2 zero;		// Vector2(0, 0) 縺ｨ蜷後§諢丞袖

	// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
	constexpr Vec2() : x(0), y(0) {};
	constexpr Vec2(const float num) : x(num), y(num) {};
	constexpr Vec2(const float x, const float y) : x(x), y(y) {};

	// 髢｢謨ｰ
	float Length() const;	// 繝吶け繝医Ν縺ｮ螟ｧ縺阪＆
	Vec2 Norm() const;	// 豁｣隕丞喧縺ｫ縺励◆繝吶け繝医Ν

	// static髢｢謨ｰ
	static float Dot(const Vec2 v1, const Vec2 v2);		// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ蜀・ｩ・
	static float Cross(const Vec2 v1, const Vec2 v2);		// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ螟也ｩ・
	static float Distance(const Vec2 v1, const Vec2 v2);	// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ霍晞屬

	// 莠後▽縺ｮ繝吶け繝医Ν縺ｧ蜷・・蛻・・荳逡ｪ螟ｧ縺阪↑蛟､繧剃ｽｿ逕ｨ縺励※繝吶け繝医Ν繧剃ｽ懈・縺吶ｋ
	static Vec2 Max(const Vec2 v1, const Vec2 v2);
	// 莠後▽縺ｮ繝吶け繝医Ν縺ｧ蜷・・蛻・・荳逡ｪ蟆上＆縺ｪ蛟､繧剃ｽｿ逕ｨ縺励※繝吶け繝医Ν繧剃ｽ懈・縺吶ｋ
	static Vec2 Min(const Vec2 v1, const Vec2 v2);

	// 邂苓｡捺ｼ皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Vec2 operator+(const Vec2 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ雜ｳ縺礼ｮ・
	Vec2 operator-(const Vec2 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ蠑輔″邂・
	Vec2 operator*(const Vec2 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ謗帙￠邂・
	Vec2 operator/(const Vec2 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ蜑ｲ繧顔ｮ・
	Vec2 operator*(float num) const;	// 荳縺､縺ｮ蛟､縺ｨ縺ｮ謗帙￠邂・
	Vec2 operator/(float num) const;	// 荳縺､縺ｮ蛟､縺ｨ縺ｮ蜑ｲ繧顔ｮ・

	// 隍・粋莉｣蜈･貍皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Vec2& operator +=(const Vec2 other); // 隍・粋莉｣蜈･貍皮ｮ・+=
	Vec2& operator -=(const Vec2 other); // 隍・粋莉｣蜈･貍皮ｮ・-=
	Vec2& operator +=(float num); // 隍・粋莉｣蜈･貍皮ｮ・+=
	Vec2& operator -=(float num); // 隍・粋莉｣蜈･貍皮ｮ・-=
	Vec2& operator *=(float num); // 隍・粋莉｣蜈･貍皮ｮ・*=
	Vec2& operator /=(float num); // 隍・粋莉｣蜈･貍皮ｮ・/=

	// 莉｣蜈･貍皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Vec2& operator=(float num);		  // 荳縺､縺ｮ蛟､繧剃ｻ｣蜈･
	Vec2& operator=(const Vec3 vec); // 荳縺､縺ｮ蛟､繧剃ｻ｣蜈･

	// 繧､繝ｳ繧ｯ繝ｪ繝｡繝ｳ繝・繝・け繝ｪ繝｡繝ｳ繝域ｼ皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Vec2& operator++(); // 蜑咲ｽｮ繧､繝ｳ繧ｯ繝ｪ繝｡繝ｳ繝・
	Vec2 operator++(int); // 蠕檎ｽｮ繧､繝ｳ繧ｯ繝ｪ繝｡繝ｳ繝・
	Vec2& operator--(); // 蜑咲ｽｮ繝・け繝ｪ繝｡繝ｳ繝・
	Vec2 operator--(int); // 蠕檎ｽｮ繝・け繝ｪ繝｡繝ｳ繝・

	// 豈碑ｼ・ｼ皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	bool operator ==(const Vec2 other);
	bool operator !=(const Vec2 other);
	bool operator >=(const Vec2 other);
	bool operator <=(const Vec2 other);
	bool operator ==(float num);
	bool operator !=(float num);
	bool operator >=(float num);
	bool operator <=(float num);
};
