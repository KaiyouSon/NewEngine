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

bool Color::operator!=(const Color other) const
{
	return r != other.r || g != other.g || b != other.b || a != other.a;
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

Color Color::operator+(const float num) const { return { r + num, g + num, b + num, a + num }; }	// ˆê‚Â‚Ì’l‚Æ‚Ì‘«‚µŽZ
Color Color::operator-(const float num) const { return { r - num, g - num, b - num, a - num }; }	// ˆê‚Â‚Ì’l‚Æ‚Ìˆø‚«ŽZ
Color Color::operator*(const float num) const { return { r * num, g * num, b * num, a * num }; }	// ˆê‚Â‚Ì’l‚Æ‚ÌŠ|‚¯ŽZ
Color Color::operator/(const float num) const { return { r / num, g / num, b / num, a / num }; }	// ˆê‚Â‚Ì’l‚Æ‚ÌŠ„‚èŽZ