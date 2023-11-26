#include "Vec2.h"
#include <math.h>

const Vec2 Vec2::left(-1.f, 0.f);
const Vec2 Vec2::right(1.f, 0.f);
const Vec2 Vec2::up(0.f, 1.f);
const Vec2 Vec2::down(0.f, -1.f);
const Vec2 Vec2::one(1.f, 1.f);
const Vec2 Vec2::zero(0.f, 0.f);

// ベクトルの長さを取得
float Vec2::Length() const { return sqrtf(x * x + y * y); }

// ベクトルを正規化して返す
Vec2 Vec2::Norm() const
{
	if (x == 0 && y == 0)
	{
		return Vec2(0, 0);
	}

	return Vec2(x / Length(), y / Length());
}

float Vec2::Area() const
{
	return x * y;
}

// 二つのベクトルの内積を計算して返す
float Vec2::Dot(const Vec2 v1, const Vec2 v2) { return v1.x * v2.x + v1.y * v2.y; }

// 二つのベクトルの外積を計算して返す
float Vec2::Cross(const Vec2 v1, const Vec2 v2) { return v1.x * v2.y - v1.y * v2.x; }

// 二つのベクトル間の距離を計算して返す
float Vec2::Distance(const Vec2 v1, const Vec2 v2)
{
	return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

// 二つのベクトルの要素ごとの最大値を返す
Vec2 Vec2::Max(const Vec2 v1, const Vec2 v2)
{
	return Vec2(
		v1.x >= v2.x ? v1.x : v2.x,
		v1.y >= v2.y ? v1.y : v2.y
	);
}

// 二つのベクトルの要素ごとの最小値を返す
Vec2 Vec2::Min(const Vec2 v1, const Vec2 v2)
{
	return Vec2(
		v1.x <= v2.x ? v1.x : v2.x,
		v1.y <= v2.y ? v1.y : v2.y
	);
}

// ベクトルの加算
Vec2 Vec2::operator+(const Vec2 other) const
{
	return Vec2(x + other.x, y + other.y);
}

// ベクトルの減算
Vec2 Vec2::operator-(const Vec2 other) const
{
	return Vec2(x - other.x, y - other.y);
}

// ベクトルの乗算
Vec2 Vec2::operator*(const Vec2 other) const
{
	return Vec2(x * other.x, y * other.y);
}

// ベクトルの除算
Vec2 Vec2::operator/(const Vec2 other) const
{
	return Vec2(x / other.x, y / other.y);
}

// スカラー倍
Vec2 Vec2::operator*(float num) const
{
	return Vec2(x * num, y * num);
}

// スカラー除算
Vec2 Vec2::operator/(float num) const
{
	return Vec2(x / num, y / num);
}

// ベクトルの加算代入
Vec2& Vec2::operator+=(const Vec2 other)
{
	x += other.x;
	y += other.y;
	return *this;
}

// ベクトルの減算代入
Vec2& Vec2::operator-=(const Vec2 other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

// スカラーの加算代入
Vec2& Vec2::operator+=(float num)
{
	x += num;
	y += num;
	return *this;
}

// スカラーの減算代入
Vec2& Vec2::operator-=(float num)
{
	x -= num;
	y -= num;
	return *this;
}

// スカラーの乗算代入
Vec2& Vec2::operator*=(float num)
{
	x *= num;
	y *= num;
	return *this;
}

// スカラーの除算代入
Vec2& Vec2::operator/=(float num)
{
	x /= num;
	y /= num;
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

bool Vec2::operator>(const Vec2 other)
{
	return x > other.x && y > other.y;
}

bool Vec2::operator<(const Vec2 other)
{
	return x < other.x && y < other.y;
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

bool Vec2::operator>(float num)
{
	return x > num && y > num;
}

bool Vec2::operator<(float num)
{
	return x < num && y < num;
}
