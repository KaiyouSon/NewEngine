#include "Color.h"
#include "Util.h"

const Color Color::sRed(255, 0, 0);
const Color Color::sGreen(0, 255, 0);
const Color Color::sBlue(0, 0, 255);
const Color Color::sYellow(255, 255, 0);
const Color Color::sWhite(255, 255, 255);
const Color Color::sBlack(0, 0, 0);

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