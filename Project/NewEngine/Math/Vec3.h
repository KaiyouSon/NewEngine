#pragma once
#include "Vec2.h"
#include "Quaternion.h"

struct Vec2;
struct Quaternion;

struct Vec3
{
	// 螟画焚
	float x; // 繝吶け繝医Ν縺ｮ x 謌仙・
	float y; // 繝吶け繝医Ν縺ｮ y 謌仙・
	float z; // 繝吶け繝医Ν縺ｮ z 謌仙・

	// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
	constexpr Vec3() : x(0), y(0), z(0) {};
	constexpr Vec3(const float num) : x(num), y(num), z(num) {};
	constexpr Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {};

	// static螟画焚
	const static Vec3 left;    // Vec3(-1,0,0) 縺ｨ蜷後§諢丞袖
	const static Vec3 right;   // Vec3(1,0,0) 縺ｨ蜷後§諢丞袖
	const static Vec3 up;      // Vec3(0,1,0) 縺ｨ蜷後§諢丞袖
	const static Vec3 down;    // Vec3(0,-1,0) 縺ｨ蜷後§諢丞袖
	const static Vec3 front;	// Vec3(0,0,1) 縺ｨ蜷後§諢丞袖
	const static Vec3 back;    // Vec3(0,0,-1)縺ｨ蜷後§諢丞袖
	const static Vec3 one;     // Vec3(1,1,1) 縺ｨ蜷後§諢丞袖
	const static Vec3 zero;    // Vec3(0,0,0) 縺ｨ蜷後§諢丞袖

	// 髢｢謨ｰ
	float Length() const;    // 繝吶け繝医Ν縺ｮ螟ｧ縺阪＆
	float LengthSq() const;    // 繝吶け繝医Ν縺ｮ螟ｧ縺阪＆
	Vec3 Norm() const;    // 豁｣隕丞喧縺ｫ縺励◆繝吶け繝医Ν

	static Vec3 Lerp(const Vec3 s, const Vec3 e, const float f);

	// static髢｢謨ｰ
	static float Dot(const Vec3 v1, const Vec3 v2);		// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ蜀・ｩ・
	static Vec3 Cross(const Vec3 v1, const Vec3 v2);		// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ螟也ｩ・
	static float Distance(const Vec3 v1, const Vec3 v2);	// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ霍晞屬
	static float DistanceSq(const Vec3 v1, const Vec3 v2);	// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ霍晞屬

	// 莠後▽縺ｮ繝吶け繝医Ν縺ｧ蜷・・蛻・・荳逡ｪ螟ｧ縺阪↑蛟､繧剃ｽｿ逕ｨ縺励※繝吶け繝医Ν繧剃ｽ懈・縺吶ｋ
	static Vec3 Max(const Vec3 v1, const Vec3 v2);
	// 莠後▽縺ｮ繝吶け繝医Ν縺ｧ蜷・・蛻・・荳逡ｪ蟆上＆縺ｪ蛟､繧剃ｽｿ逕ｨ縺励※繝吶け繝医Ν繧剃ｽ懈・縺吶ｋ
	static Vec3 Min(const Vec3 v1, const Vec3 v2);

	// 邂苓｡捺ｼ皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Vec3 operator+(const Vec3 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ雜ｳ縺礼ｮ・
	Vec3 operator-(const Vec3 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ蠑輔″邂・
	Vec3 operator*(const Vec3 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ謗帙￠邂・
	Vec3 operator/(const Vec3 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ蜑ｲ繧顔ｮ・
	Vec3 operator+(const Vec2 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ雜ｳ縺礼ｮ・
	Vec3 operator-(const Vec2 other) const; // 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ蠑輔″邂・
	Vec3 operator*(const Vec2 other) const; // 荳縺､縺ｮ蛟､縺ｨ縺ｮ謗帙￠邂・
	Vec3 operator/(const Vec2 other) const; // 荳縺､縺ｮ蛟､縺ｨ縺ｮ蜑ｲ繧顔ｮ・
	Vec3 operator+(float num) const;		 // 荳縺､縺ｮ蛟､縺ｨ縺ｮ雜ｳ縺礼ｮ・
	Vec3 operator-(float num) const;		 // 荳縺､縺ｮ蛟､縺ｨ縺ｮ蠑輔″邂・
	Vec3 operator*(float num) const;         // 荳縺､縺ｮ蛟､縺ｨ縺ｮ謗帙￠邂・
	Vec3 operator/(float num) const;         // 荳縺､縺ｮ蛟､縺ｨ縺ｮ蜑ｲ繧顔ｮ・
	Vec3 operator-() const;

	// 隍・粋莉｣蜈･貍皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Vec3& operator+=(const Vec3 other); // 隍・粋莉｣蜈･貍皮ｮ・+=
	Vec3& operator-=(const Vec3 other); // 隍・粋莉｣蜈･貍皮ｮ・-=
	Vec3& operator+=(float num);         // 隍・粋莉｣蜈･貍皮ｮ・+=
	Vec3& operator-=(float num);         // 隍・粋莉｣蜈･貍皮ｮ・-=
	Vec3& operator*=(float num);         // 隍・粋莉｣蜈･貍皮ｮ・*=
	Vec3& operator/=(float num);         // 隍・粋莉｣蜈･貍皮ｮ・/=

	// 莉｣蜈･貍皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Vec3& operator=(float num);			// 荳縺､縺ｮ蛟､繧剃ｻ｣蜈･
	Vec3& operator=(const Vec2 other); // Vec2繧歎ec3縺ｫ螟画鋤
	Vec3& operator=(const Quaternion& q); // Quaternion繧歎ec3縺ｫ螟画鋤

	// 繧､繝ｳ繧ｯ繝ｪ繝｡繝ｳ繝・繝・け繝ｪ繝｡繝ｳ繝域ｼ皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Vec3& operator++();   // 蜑咲ｽｮ繧､繝ｳ繧ｯ繝ｪ繝｡繝ｳ繝・
	Vec3 operator++(int); // 蠕檎ｽｮ繧､繝ｳ繧ｯ繝ｪ繝｡繝ｳ繝・
	Vec3& operator--();   // 蜑咲ｽｮ繝・け繝ｪ繝｡繝ｳ繝・
	Vec3 operator--(int); // 蠕檎ｽｮ繝・け繝ｪ繝｡繝ｳ繝・

	// 豈碑ｼ・ｼ皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	bool operator==(const Vec3 other);
	bool operator!=(const Vec3 other);
	bool operator>=(const Vec3 other);
	bool operator<=(const Vec3 other);
	bool operator==(float num);
	bool operator!=(float num);
	bool operator>=(float num);
	bool operator<=(float num);
};
