#include "Vec2.h"
#include <math.h>

const Vec2 Vec2::left(-1.f, 0.f);
const Vec2 Vec2::right(1.f, 0.f);
const Vec2 Vec2::up(0.f, 1.f);
const Vec2 Vec2::down(0.f, -1.f);
const Vec2 Vec2::one(1.f, 1.f);
const Vec2 Vec2::zero(0.f, 0.f);

// 繝吶け繝医Ν縺ｮ螟ｧ縺阪＆
float Vec2::Length() const { return sqrtf(x * x + y * y); }

// 豁｣隕丞喧・磯聞縺輔ｒ・代↓縺励◆・峨・繧ｯ繝医Ν
Vec2 Vec2::Norm() const
{
	if (x == 0 && y == 0)
	{
		return 0;
	}

	return{ x / Length() , y / Length() };
}

// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ蜀・ｩ・
float Vec2::Dot(const Vec2 v1, const Vec2 v2) { return v1.x * v2.x + v1.y * v2.y; }

// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ螟也ｩ・
float Vec2::Cross(const Vec2 v1, const Vec2 v2) { return v1.x * v2.y - v1.y * v2.x; }

// 莠後▽縺ｮ繝吶け繝医Ν縺ｮ霍晞屬
float Vec2::Distance(const Vec2 v1, const Vec2 v2)
{
	return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

// 莠後▽縺ｮ繝吶け繝医Ν縺ｧ蜷・・蛻・・荳逡ｪ螟ｧ縺阪↑蛟､繧剃ｽｿ逕ｨ縺励※繝吶け繝医Ν繧剃ｽ懈・縺吶ｋ
Vec2 Vec2::Max(const Vec2 v1, const Vec2 v2)
{
	return Vec2(
		v1.x >= v2.x ? v1.x : v2.x,
		v1.y >= v2.y ? v1.y : v2.y);
}

// 莠後▽縺ｮ繝吶け繝医Ν縺ｧ蜷・・蛻・・荳逡ｪ蟆上＆縺ｪ蛟､繧剃ｽｿ逕ｨ縺励※繝吶け繝医Ν繧剃ｽ懈・縺吶ｋ
Vec2 Vec2::Min(const Vec2 v1, const Vec2 v2)
{
	return Vec2(
		v1.x <= v2.x ? v1.x : v2.x,
		v1.y <= v2.y ? v1.y : v2.y);
}

// 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ雜ｳ縺礼ｮ・
Vec2 Vec2::operator+(const Vec2 other) const
{
	return { x + other.x , y + other.y };
}

// 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ蠑輔″邂・
Vec2 Vec2::operator-(const Vec2 other) const
{
	return { x - other.x , y - other.y };
}

// 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ謗帙￠邂・
Vec2 Vec2::operator*(const Vec2 other) const
{
	return { x * other.x , y * other.y };
}

// 繧ゅ≧荳譁ｹ縺ｮ繝吶け繝医Ν縺ｨ縺ｮ蜑ｲ繧顔ｮ・
Vec2 Vec2::operator/(const Vec2 other) const
{
	return { x / other.x , y / other.y };
}

// 荳縺､縺ｮ蛟､縺ｨ縺ｮ謗帙￠邂・
Vec2 Vec2:: operator*(float num)const
{
	return { x * num,y * num };
}

// 荳縺､縺ｮ蛟､縺ｨ縺ｮ蜑ｲ繧顔ｮ・
Vec2 Vec2::operator/(float num)const
{
	return { x / num,y / num };
}

// 隍・粋莉｣蜈･貍皮ｮ・+=
Vec2& Vec2::operator+=(const Vec2 other)
{
	x += other.x;
	y += other.y;
	return *this;
}

// 隍・粋莉｣蜈･貍皮ｮ・-=
Vec2& Vec2::operator-=(const Vec2 other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vec2& Vec2::operator+=(float num)
{
	x += num;
	y += num;
	return *this;
}

Vec2& Vec2::operator-=(float num)
{
	x -= num;
	y -= num;
	return *this;
}

// 隍・粋莉｣蜈･貍皮ｮ・*=
Vec2& Vec2::operator*=(float num)
{
	x *= num;
	y *= num;
	return *this;
}

// 隍・粋莉｣蜈･貍皮ｮ・/=
Vec2& Vec2::operator/=(float num)
{
	x += num;
	y += num;
	return *this;
}

Vec2& Vec2::operator=(float num)
{
	x = num;
	y = num;
	return *this;
}

Vec2& Vec2::operator=(const Vec3 vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}

Vec2& Vec2::operator++()
{
	x++;
	y++;
	return *this;
}

Vec2 Vec2::operator++(int)
{
	Vec2 tmp = *this;
	++* this;
	return tmp;
}

Vec2& Vec2::operator--()
{
	x--;
	y--;
	return *this;
}

Vec2 Vec2::operator--(int)
{
	Vec2 tmp = *this;
	--* this;
	return tmp;
}

bool Vec2::operator==(const Vec2 other)
{
	return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2 other)
{
	return x != other.x || y != other.y;
}

bool Vec2::operator>=(const Vec2 other)
{
	return x >= other.x && y >= other.y;
}

bool Vec2::operator<=(const Vec2 other)
{
	return x <= other.x && y <= other.y;
}

bool Vec2::operator==(float num)
{
	return x == num && y == num;
}

bool Vec2::operator!=(float num)
{
	return x != num || y != num;
}

bool Vec2::operator>=(float num)
{
	return x >= num && y >= num;
}

bool Vec2::operator<=(float num)
{
	return x <= num && y <= num;

}

