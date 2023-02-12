#include "Vec3.h"
#include <math.h>

const Vec3 Vec3::left(-1, 0, 0);
const Vec3 Vec3::right(1, 0, 0);
const Vec3 Vec3::up(0, 1, 0);
const Vec3 Vec3::down(0, -1, 0);
const Vec3 Vec3::front(0, 0, 1);
const Vec3 Vec3::back(0, 0, -1);
const Vec3 Vec3::one(1, 1, 1);
const Vec3 Vec3::zero(0, 0, 0);

float Vec3::Length() const { return sqrtf(x * x + y * y + z * z); }

Vec3 Vec3::Norm() const { return { x / Length(), y / Length(), z / Length() }; }

float Vec3::Dot(const Vec3& v1, const Vec3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

Vec3 Vec3::Cross(const Vec3& v1, const Vec3& v2)
{
	return {
		(v1.y * v2.z - v1.z * v2.y),
		(v1.z * v2.x - v1.x * v2.z),
		(v1.x * v2.y - v1.y * v2.x) };
}

float Vec3::Distance(const Vec3& v1, const Vec3& v2)
{
	return sqrtf(
		(v2.x - v1.x) * (v2.x - v1.x) +
		(v2.y - v1.y) * (v2.y - v1.y) +
		(v2.z - v1.z) * (v2.z - v1.z));
}

Vec3 Vec3::Max(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.x >= v2.x ? v1.x : v2.x, v1.y >= v2.y ? v1.y : v2.y, v1.z >= v2.z ? v1.z : v2.z);
}

Vec3 Vec3::Min(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.x <= v2.x ? v1.x : v2.x, v1.y <= v2.y ? v1.y : v2.z, v1.z <= v2.z ? v1.z : v2.z);
}

Vec3 Vec3::operator+(const Vec3& other) const { return { x + other.x, y + other.y, z + other.z }; }
Vec3 Vec3::operator-(const Vec3& other) const { return { x - other.x, y - other.y, z - other.z }; }
Vec3 Vec3::operator*(const Vec3& other) const { return { x * other.x, y * other.y, z * other.z }; }
Vec3 Vec3::operator/(const Vec3& other) const { return { x / other.x, y / other.y, z / other.z }; }
Vec3 Vec3::operator+(const Vec2& other) const { return { x + other.x, y + other.y ,z }; }
Vec3 Vec3::operator-(const Vec2& other) const { return { x - other.x, y - other.y ,z }; }
Vec3 Vec3::operator*(const Vec2& other) const { return { x * other.x, y * other.y ,z }; }
Vec3 Vec3::operator/(const Vec2& other) const { return { x / other.x, y / other.y ,z }; }
Vec3 Vec3::operator+(float num) const { return { x + num, y + num, z + num }; }	// ��̒l�Ƃ̑����Z
Vec3 Vec3::operator-(float num) const { return { x - num, y - num, z - num }; }	// ��̒l�Ƃ̈����Z
Vec3 Vec3::operator*(float num) const { return { x * num, y * num, z * num }; }	// ��̒l�Ƃ̊|���Z
Vec3 Vec3::operator/(float num) const { return { x / num, y / num, z / num }; }	// ��̒l�Ƃ̊���Z

Vec3 Vec3::operator-() const { return { -x,-y,-z }; }

// ����������Z +=
Vec3& Vec3::operator+=(const Vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// ����������Z -=
Vec3& Vec3::operator-=(const Vec3& other)
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

// ����������Z *=
Vec3& Vec3::operator*=(float num)
{
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

// ����������Z /=
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

Vec3& Vec3::operator=(const Vec2& other)
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

bool Vec3::operator==(const Vec3& other) { return x == other.x && y == other.y && z == other.z; }

bool Vec3::operator!=(const Vec3& other) { return x != other.x || y != other.y || z != other.z; }

bool Vec3::operator>=(const Vec3& other) { return x >= other.x && y >= other.y && z >= other.z; }

bool Vec3::operator<=(const Vec3& other) { return x <= other.x && y <= other.y && z <= other.z; }

bool Vec3::operator==(float num) { return x == num && y == num && z == num; }

bool Vec3::operator!=(float num) { return x != num || y != num || z != num; }

bool Vec3::operator>=(float num) { return x >= num && y >= num && z >= num; }

bool Vec3::operator<=(float num) { return x <= num && y <= num && z <= num; }