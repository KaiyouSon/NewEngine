#include "Color.h"
#include "Util.h"

const Color Color::red(255, 0, 0);
const Color Color::green(0, 255, 0);
const Color Color::blue(0, 0, 255);
const Color Color::yellow(255, 255, 0);
const Color Color::white(255, 255, 255);
const Color Color::black(0, 0, 0);

bool Color::operator!=(const Color& other) const
{
	return r != other.r || g != other.g || b != other.b || a != other.a;
}

Color Color::operator=(const Color& other)
{
	r = other.r, g = other.g, b = other.b, a = other.a;
	return *this;
}

Color& Color::operator++()
{
	r++;
	g++;
	b++;
	return *this;
}

Color Color::operator+(float num) const { return { r + num, g + num, b + num, a + num }; }	// ��̒l�Ƃ̑����Z
Color Color::operator-(float num) const { return { r - num, g - num, b - num, a - num }; }	// ��̒l�Ƃ̈����Z
Color Color::operator*(float num) const { return { r * num, g * num, b * num, a * num }; }	// ��̒l�Ƃ̊|���Z
Color Color::operator/(float num) const { return { r / num, g / num, b / num, a / num }; }	// ��̒l�Ƃ̊���Z