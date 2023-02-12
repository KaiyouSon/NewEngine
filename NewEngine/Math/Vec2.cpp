#include "Vec2.h"
#include <math.h>

const Vec2 Vec2::left(-1, 0);
const Vec2 Vec2::right(1, 0);
const Vec2 Vec2::up(0, 1);
const Vec2 Vec2::down(0, -1);
const Vec2 Vec2::one(1, 1);
const Vec2 Vec2::zero(0, 0);

// �x�N�g���̑傫��
float Vec2::Length() const { return sqrtf(x * x + y * y); }

// ���K���i�������P�ɂ����j�x�N�g��
Vec2 Vec2::Norm() const { return{ x / Length() , y / Length() }; }

// ��̃x�N�g���̓���
float Vec2::Dot(const Vec2& v1, const Vec2& v2) { return v1.x * v2.x + v1.y * v2.y; }

// ��̃x�N�g���̊O��
float Vec2::Cross(const Vec2& v1, const Vec2& v2) { return v1.x * v2.y - v1.y * v2.x; }

// ��̃x�N�g���̋���
float Vec2::Distance(const Vec2& v1, const Vec2& v2)
{
	return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

// ��̃x�N�g���Ŋe�����̈�ԑ傫�Ȓl���g�p���ăx�N�g�����쐬����
Vec2 Vec2::Max(const Vec2& v1, const Vec2& v2)
{
	return Vec2(
		v1.x >= v2.x ? v1.x : v2.x,
		v1.y >= v2.y ? v1.y : v2.y);
}

// ��̃x�N�g���Ŋe�����̈�ԏ����Ȓl���g�p���ăx�N�g�����쐬����
Vec2 Vec2::Min(const Vec2& v1, const Vec2& v2)
{
	return Vec2(
		v1.x <= v2.x ? v1.x : v2.x,
		v1.y <= v2.y ? v1.y : v2.y);
}

// ��������̃x�N�g���Ƃ̑����Z
Vec2 Vec2::operator+(const Vec2& other) const
{
	return { x + other.x , y + other.y };
}

// ��������̃x�N�g���Ƃ̈����Z
Vec2 Vec2::operator-(const Vec2& other) const
{
	return { x - other.x , y - other.y };
}

// ��������̃x�N�g���Ƃ̊|���Z
Vec2 Vec2::operator*(const Vec2& other) const
{
	return { x * other.x , y * other.y };
}

// ��������̃x�N�g���Ƃ̊���Z
Vec2 Vec2::operator/(const Vec2& other) const
{
	return { x / other.x , y / other.y };
}

// ��̒l�Ƃ̊|���Z
Vec2 Vec2:: operator*(float num)const
{
	return { x * num,y * num };
}

// ��̒l�Ƃ̊���Z
Vec2 Vec2::operator/(float num)const
{
	return { x / num,y / num };
}

// ����������Z +=
Vec2& Vec2::operator+=(const Vec2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

// ����������Z -=
Vec2& Vec2::operator-=(const Vec2& other)
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

// ����������Z *=
Vec2& Vec2::operator*=(float num)
{
	x *= num;
	y *= num;
	return *this;
}

// ����������Z /=
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

Vec2& Vec2::operator=(const Vec3& vec)
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

bool Vec2::operator==(const Vec2& other)
{
	return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2& other)
{
	return x != other.x || y != other.y;
}

bool Vec2::operator>=(const Vec2& other)
{
	return x >= other.x && y >= other.y;
}

bool Vec2::operator<=(const Vec2& other)
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
