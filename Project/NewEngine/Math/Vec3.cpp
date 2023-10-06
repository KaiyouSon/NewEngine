#include "Vec3.h"
#include <math.h>

const Vec3 Vec3::left(-1.f, 0.f, 0.f);
const Vec3 Vec3::right(1.f, 0.f, 0.f);
const Vec3 Vec3::up(0.f, 1.f, 0.f);
const Vec3 Vec3::down(0.f, -1.f, 0.f);
const Vec3 Vec3::front(0.f, 0.f, 1.f);
const Vec3 Vec3::back(0.f, 0.f, -1.f);
const Vec3 Vec3::one(1.f, 1.f, 1.f);
const Vec3 Vec3::zero(0.f, 0.f, 0.f);

float Vec3::Length() const { return sqrtf(x * x + y * y + z * z); }

float Vec3::LengthSq() const { return x * x + y * y + z * z; }

Vec3 Vec3::Norm() const
{
	if (x == 0 && y == 0 && z == 0)
	{
		return 0;
	}

	return{ x / Length(), y / Length(), z / Length() };
}

Vec3 Vec3::Lerp(const Vec3 s, const Vec3 e, const float f)
{
	Vec3 dis = e - s;
	return dis * f + s;
}

float Vec3::Dot(const Vec3 v1, const Vec3 v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

Vec3 Vec3::Cross(const Vec3 v1, const Vec3 v2)
{
	return {
		(v1.y * v2.z - v1.z * v2.y),
		(v1.z * v2.x - v1.x * v2.z),
		(v1.x * v2.y - v1.y * v2.x) };
}

float Vec3::Distance(const Vec3 v1, const Vec3 v2)
{
	return sqrtf(
		(v2.x - v1.x) * (v2.x - v1.x) +
		(v2.y - v1.y) * (v2.y - v1.y) +
		(v2.z - v1.z) * (v2.z - v1.z));
}

float Vec3::DistanceSq(const Vec3 v1, const Vec3 v2)
{
	return
		(v2.x - v1.x) * (v2.x - v1.x) +
		(v2.y - v1.y) * (v2.y - v1.y) +
		(v2.z - v1.z) * (v2.z - v1.z);
}

Vec3 Vec3::Max(const Vec3 v1, const Vec3 v2)
{
	return Vec3(v1.x >= v2.x ? v1.x : v2.x, v1.y >= v2.y ? v1.y : v2.y, v1.z >= v2.z ? v1.z : v2.z);
}

Vec3 Vec3::Min(const Vec3 v1, const Vec3 v2)
{
	return Vec3(v1.x <= v2.x ? v1.x : v2.x, v1.y <= v2.y ? v1.y : v2.z, v1.z <= v2.z ? v1.z : v2.z);
}

Vec3 Vec3::operator+(const Vec3 other) const { return { x + other.x, y + other.y, z + other.z }; }
Vec3 Vec3::operator-(const Vec3 other) const { return { x - other.x, y - other.y, z - other.z }; }
Vec3 Vec3::operator*(const Vec3 other) const { return { x * other.x, y * other.y, z * other.z }; }
Vec3 Vec3::operator/(const Vec3 other) const { return { x / other.x, y / other.y, z / other.z }; }
Vec3 Vec3::operator+(const Vec2 other) const { return { x + other.x, y + other.y ,z }; }
Vec3 Vec3::operator-(const Vec2 other) const { return { x - other.x, y - other.y ,z }; }
Vec3 Vec3::operator*(const Vec2 other) const { return { x * other.x, y * other.y ,z }; }
Vec3 Vec3::operator/(const Vec2 other) const { return { x / other.x, y / other.y ,z }; }
Vec3 Vec3::operator+(float num) const { return { x + num, y + num, z + num }; }	// 荳縺､縺ｮ蛟､縺ｨ縺ｮ雜ｳ縺礼ｮ・
Vec3 Vec3::operator-(float num) const { return { x - num, y - num, z - num }; }	// 荳縺､縺ｮ蛟､縺ｨ縺ｮ蠑輔″邂・
Vec3 Vec3::operator*(float num) const { return { x * num, y * num, z * num }; }	// 荳縺､縺ｮ蛟､縺ｨ縺ｮ謗帙￠邂・
Vec3 Vec3::operator/(float num) const { return { x / num, y / num, z / num }; }	// 荳縺､縺ｮ蛟､縺ｨ縺ｮ蜑ｲ繧顔ｮ・

Vec3 Vec3::operator-() const { return { -x,-y,-z }; }

// 隍・粋莉｣蜈･貍皮ｮ・+=
Vec3& Vec3::operator+=(const Vec3 other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// 隍・粋莉｣蜈･貍皮ｮ・-=
Vec3& Vec3::operator-=(const Vec3 other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vec3& Vec3::operator+=(float num)
{
	x += num;
	y += num;
	z += num;
	return *this;
}

Vec3& Vec3::operator-=(float num)
{
	x -= num;
	y -= num;
	z -= num;
	return *this;
}

// 隍・粋莉｣蜈･貍皮ｮ・*=
Vec3& Vec3::operator*=(float num)
{
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

// 隍・粋莉｣蜈･貍皮ｮ・/=
Vec3& Vec3::operator/=(float num)
{
	x += num;
	y += num;
	z += num;
	return *this;
}

Vec3& Vec3::operator=(float num)
{
	x = num;
	y = num;
	z = num;
	return *this;
}

Vec3& Vec3::operator=(const Vec2 other)
{
	x = other.x;
	y = other.y;
	return *this;
}

Vec3& Vec3::operator=(const Quaternion& q)
{
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}

Vec3& Vec3::operator++()
{
	x++;
	y++;
	z++;
	return *this;
}

Vec3 Vec3::operator++(int)
{
	Vec3 tmp = *this;
	++* this;
	return tmp;
}

Vec3& Vec3::operator--()
{
	x--;
	y--;
	z--;
	return *this;
}

Vec3 Vec3::operator--(int)
{
	Vec3 tmp = *this;
	--* this;
	return tmp;
}

bool Vec3::operator==(const Vec3 other) { return x == other.x && y == other.y && z == other.z; }

bool Vec3::operator!=(const Vec3 other) { return x != other.x || y != other.y || z != other.z; }

bool Vec3::operator>=(const Vec3 other) { return x >= other.x && y >= other.y && z >= other.z; }

bool Vec3::operator<=(const Vec3 other) { return x <= other.x && y <= other.y && z <= other.z; }

bool Vec3::operator==(float num) { return x == num && y == num && z == num; }

bool Vec3::operator!=(float num) { return x != num || y != num || z != num; }

bool Vec3::operator>=(float num) { return x >= num && y >= num && z >= num; }

bool Vec3::operator<=(float num) { return x <= num && y <= num && z <= num; }
