#include "Color.h"
#include "Util.h"

const Color Color::red(255.f, 0.f, 0.f);
const Color Color::green(0.f, 255.f, 0.f);
const Color Color::blue(0.f, 0.f, 255.f);
const Color Color::yellow(255.f, 255.f, 0.f);
const Color Color::white(255.f, 255.f, 255.f);
const Color Color::black(0.f, 0.f, 0.f);
const Color Color::one(1.f, 1.f, 1.f);
const Color Color::zero(0.f, 0.f, 0.f);

Color::Color() : r(255.f), g(255.f), b(255.f), a(255.f) {}
Color::Color(float r, float g, float b) : r(r), g(g), b(b), a(255.f) {}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
Color::Color(const uint32_t colorCodo)
{
	r = (float)((colorCodo >> 16) & 0xff);
	g = (float)((colorCodo >> 8) & 0xff);
	b = (float)(colorCodo & 0xff);
	a = 255;
}

bool Color::operator!=(const Color other) const
{
	return r != other.r || g != other.g || b != other.b || a != other.a;
}

Color Color::To01()
{
	return *this / 255.f;
}

Color Color::operator=(const Color other)
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

Color Color::operator+(const float num) const { return { r + num, g + num, b + num, a + num }; }
Color Color::operator-(const float num) const { return { r - num, g - num, b - num, a - num }; }
Color Color::operator*(const float num) const { return { r * num, g * num, b * num, a * num }; }
Color Color::operator/(const float num) const { return { r / num, g / num, b / num, a / num }; }
