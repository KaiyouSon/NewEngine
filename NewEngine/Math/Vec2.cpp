#include "Vec2.h"
#include <math.h>

const Vec2 Vec2::left(-1.f, 0.f);
const Vec2 Vec2::right(1.f, 0.f);
const Vec2 Vec2::up(0.f, 1.f);
const Vec2 Vec2::down(0.f, -1.f);
const Vec2 Vec2::one(1.f, 1.f);
const Vec2 Vec2::zero(0.f, 0.f);

// ベクトルの大きさ
float Vec2::Length() const { return sqrtf(x * x + y * y); }

// 正規化（長さを１にした）ベクトル
Vec2 Vec2::Norm() const { return{ x / Length() , y / Length() }; }

// 二つのベクトルの内積
float Vec2::Dot(const Vec2 v1, const Vec2 v2) { return v1.x * v2.x + v1.y * v2.y; }

// 二つのベクトルの外積
float Vec2::Cross(const Vec2 v1, const Vec2 v2) { return v1.x * v2.y - v1.y * v2.x; }

// 二つのベクトルの距離
float Vec2::Distance(const Vec2 v1, const Vec2 v2)
{
	return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

// 二つのベクトルで各成分の一番大きな値を使用してベクトルを作成する
Vec2 Vec2::Max(const Vec2 v1, const Vec2 v2)
{
	return Vec2(
		v1.x >= v2.x ? v1.x : v2.x,
		v1.y >= v2.y ? v1.y : v2.y);
}

// 二つのベクトルで各成分の一番小さな値を使用してベクトルを作成する
Vec2 Vec2::Min(const Vec2 v1, const Vec2 v2)
{
	return Vec2(
		v1.x <= v2.x ? v1.x : v2.x,
		v1.y <= v2.y ? v1.y : v2.y);
}

// もう一方のベクトルとの足し算
Vec2 Vec2::operator+(const Vec2 other) const
{
	return { x + other.x , y + other.y };
}

// もう一方のベクトルとの引き算
Vec2 Vec2::operator-(const Vec2 other) const
{
	return { x - other.x , y - other.y };
}

// もう一方のベクトルとの掛け算
Vec2 Vec2::operator*(const Vec2 other) const
{
	return { x * other.x , y * other.y };
}

// もう一方のベクトルとの割り算
Vec2 Vec2::operator/(const Vec2 other) const
{
	return { x / other.x , y / other.y };
}

// 一つの値との掛け算
Vec2 Vec2:: operator*(float num)const
{
	return { x * num,y * num };
}

// 一つの値との割り算
Vec2 Vec2::operator/(float num)const
{
	return { x / num,y / num };
}

// 複合代入演算 +=
Vec2& Vec2::operator+=(const Vec2 other)
{
	x += other.x;
	y += other.y;
	return *this;
}

// 複合代入演算 -=
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

// 複合代入演算 *=
Vec2& Vec2::operator*=(float num)
{
	x *= num;
	y *= num;
	return *this;
}

// 複合代入演算 /=
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
