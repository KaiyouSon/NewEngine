#pragma once
#include <cstdint>

struct Color
{
	float r, g, b, a;

	constexpr Color() : r(255.f), g(255.f), b(255.f), a(255.f) {}
	constexpr Color(float r, float g, float b) : r(r), g(g), b(b), a(255.f) {}
	constexpr Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	constexpr Color(const uint32_t colorCodo)
	{
		r = (float)((colorCodo >> 16) & 0xff);
		g = (float)((colorCodo >> 8) & 0xff);
		b = (float)(colorCodo & 0xff);
		a = 255;
	}

	const static Color red;
	const static Color green;
	const static Color blue;
	const static Color yellow;
	const static Color white;
	const static Color black;
	const static Color one;
	const static Color zero;

	Color To01();

	Color operator=(const Color other);

	// 豈碑ｼ・ｼ皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	bool operator!=(const Color other) const;
	Color& operator++();

	Color operator+(const float num) const;	// 荳縺､縺ｮ蛟､縺ｨ縺ｮ雜ｳ縺礼ｮ・
	Color operator-(const float num) const;	// 荳縺､縺ｮ蛟､縺ｨ縺ｮ蠑輔″邂・
	Color operator*(const float num) const; // 荳縺､縺ｮ蛟､縺ｨ縺ｮ謗帙￠邂・
	Color operator/(const float num) const; // 荳縺､縺ｮ蛟､縺ｨ縺ｮ蜑ｲ繧顔ｮ・

};
