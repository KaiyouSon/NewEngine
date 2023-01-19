#include "Quaternion.h"
#include "MathUtil.h"

float Quaternion::Lenght() const { return sqrt(x * x + y * y + z * z + w * w); }

Quaternion Quaternion::Norm() const { return { x / Lenght(),y / Lenght(),z / Lenght() ,w / Lenght() }; }


Quaternion Quaternion::Conjugate() const { return { -x,-y,-z,w }; }

Quaternion Quaternion::Inverse() const
{
	return Conjugate() / (Lenght() * Lenght());
}

Quaternion Quaternion::AnyAxisRotation(const Vec3& v, const float& radian)
{
	Quaternion result = { x,y,z,w };

	// 回転クォータニオン作成
	Quaternion q =
	{
		v.Norm().x * sinf(radian / 2),
		v.Norm().y * sinf(radian / 2),
		v.Norm().z * sinf(radian / 2),
		cosf(radian / 2)
	};
	// 回転クォータニオンの共役クォータニオン作成      
	Quaternion qc = q.Conjugate();

	// 回転処理
	result = q * result;
	result = result * qc;

	return result;
}

float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	return { (q1.w * q2.w) + (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) };
}

Quaternion Quaternion::Identity() { return { 0, 0, 0, 1 }; }

Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return { x + other.x,y + other.y,z + other.z,w + other.w };
}

Quaternion Quaternion::operator-(const Quaternion& other) const
{
	return { x - other.x,y - other.y,z - other.z,w - other.w };
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	Quaternion result;
	float num[4] = {};

	num[0] = +w * other.w;
	num[1] = -x * other.x;
	num[2] = -y * other.y;
	num[3] = -z * other.z;
	result.w = num[0] + num[1] + num[2] + num[3];

	num[0] = +w * other.x;
	num[1] = +other.w * x;
	num[2] = +y * other.z;
	num[3] = -z * other.y;
	result.x = num[0] + num[1] + num[2] + num[3];

	num[0] = +w * other.y;
	num[1] = +other.w * y;
	num[2] = +z * other.x;
	num[3] = -x * other.z;
	result.y = num[0] + num[1] + num[2] + num[3];

	num[0] = +w * other.z;
	num[1] = +other.w * z;
	num[2] = +x * other.y;
	num[3] = -y * other.x;
	result.z = num[0] + num[1] + num[2] + num[3];

	return result;
}

Quaternion Quaternion::operator/(const Quaternion& other) const
{
	return Quaternion();
}

Quaternion Quaternion::operator*(const float& num) const
{
	return { x * num,y * num ,z * num ,w * num };
}

Quaternion Quaternion::operator/(const float& num) const
{
	return { x / num,y / num ,z / num ,w / num };
}

Quaternion& Quaternion::operator+=(const Quaternion& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& other)
{
	Quaternion result = *this * other;
	return result;
}

Quaternion& Quaternion::operator*=(const float& num)
{
	x *= num;
	y *= num;
	z *= num;
	w *= num;
	return *this;
}

Quaternion& Quaternion::operator/=(const float& num)
{
	x /= num;
	y /= num;
	z /= num;
	w /= num;
	return *this;
}
