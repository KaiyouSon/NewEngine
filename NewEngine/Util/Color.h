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

	Color GetColorTo01() { return { r / 255,g / 255,b / 255 ,a / 255 }; }

	Color operator=(const Color other);

	// ��r���Z�q�̃I�[�o�[���[�h
	bool operator!=(const Color other) const;
	Color& operator++();

	Color operator+(const float num) const;		 // ��̒l�Ƃ̑����Z
	Color operator-(const float num) const;		 // ��̒l�Ƃ̈����Z
	Color operator*(const float num) const;         // ��̒l�Ƃ̊|���Z
	Color operator/(const float num) const;         // ��̒l�Ƃ̊���Z

};

