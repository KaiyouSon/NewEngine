#pragma once
#include <cstdint>

// 色の構造体
struct Color
{
	float r, g, b, a;

	Color();
	Color(float r, float g, float b);
	Color(float r, float g, float b, float a);
	Color(const uint32_t colorCodo);

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

	bool operator!=(const Color other) const;
	Color& operator++();

	Color operator+(const float num) const;
	Color operator-(const float num) const;
	Color operator*(const float num) const;
	Color operator/(const float num) const;

};
