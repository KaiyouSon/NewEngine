#pragma once
class Color
{
public:
	float r, g, b, a;

	constexpr Color() : r(255), g(255), b(255), a(255) {}
	constexpr Color(float r, float g, float b) : r(r), g(g), b(b), a(255) {}
	constexpr Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	const static Color red;
	const static Color green;
	const static Color blue;
	const static Color yellow;
	const static Color white;
	const static Color black;

	Color GetColorTo01() { return { r / 255,g / 255,b / 255 ,a / 255 }; }

	Color operator=(const Color& other);

	// 比較演算子のオーバーロード
	bool operator!=(const Color& other) const;
	Color& operator++();

	Color operator+(float num) const;		 // 一つの値との足し算
	Color operator-(float num) const;		 // 一つの値との引き算
	Color operator*(float num) const;         // 一つの値との掛け算
	Color operator/(float num) const;         // 一つの値との割り算

};

