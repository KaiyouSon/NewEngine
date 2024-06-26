#include "Quaternion.h"
#include "MathUtil.h"
#include <math.h>

Quaternion::Quaternion(const Vec3 v) : x(v.x), y(v.y), z(v.z), w(0)
{
}

float Quaternion::Length() const { return sqrtf(x * x + y * y + z * z + w * w); }

Quaternion Quaternion::Norm() const { return { x / Length(),y / Length(),z / Length() ,w / Length() }; }

Quaternion Quaternion::Conjugate() const { return { -x,-y,-z,w }; }

Quaternion Quaternion::Inverse() const
{
	return Conjugate() / (Length() * Length());
}

// 任意軸回転を作成
Quaternion Quaternion::AnyAxisRotation(const Vec3 v, const float radians)
{
	Quaternion result = { x, y, z, w };

	// 軸を正規化
	Quaternion q =
	{
		v.Norm().x * sinf(radians / 2),
		v.Norm().y * sinf(radians / 2),
		v.Norm().z * sinf(radians / 2),
		cosf(radians / 2)
	};
	// 軸回転がクォータニオンの逆クォータニオンを作成
	Quaternion qc = q.Conjugate();

	// 軸回転を適用
	result = q * result;
	result = result * qc;

	return result;
}

float Quaternion::Dot(const Quaternion q1, const Quaternion q2)
{
	return { (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w) };
}

Quaternion Quaternion::Identity() { return { 0, 0, 0, 1 }; }

Quaternion Quaternion::Slerp(const Quaternion q1, const Quaternion q2, float t)
{
	float cos = Quaternion::Dot(q1, q2);
	Quaternion t2 = q2;

	if (cos < 0.f)
	{
		cos = -cos;
		t2 = -q2;
	}

	float k0 = 1.f - t;
	float k1 = t;

	if ((1.f - cos) > 0.001f)
	{
		float theta = acosf(cos);
		k0 = sinf(theta * k0) / sinf(theta);
		k1 = sinf(theta * k1) / sinf(theta);
	}

	if (cos >= 1.f - 0.0005f)
	{
		return  q1 * (1.f - t) + t2 * t;
	}

	return q1 * k0 + t2 * k1;
}

Quaternion Quaternion::MakeAxisAngle(const Vec3 v, const float radians)
{
	// 軸回転を作成
	Quaternion q =
	{
		v.Norm().x * sinf(radians / 2),
		v.Norm().y * sinf(radians / 2),
		v.Norm().z * sinf(radians / 2),
		cosf(radians / 2)
	};

	return q;
}

Quaternion Quaternion::DirectionToDirection(const Vec3 v1, const Vec3 v2)
{
	// uとvベクトルの内積を計算
	// uとvが正規化されている必要があります
	Vec3 u = v1;
	u.Norm();
	Vec3 v = v2;
	v.Norm();

	float dot = Vec3::Dot(u, v);

	// uとvベクトルの外積を計算
	// 霆角がベクトルに含まれ、uとvの両方に垂直です
	Vec3 cross = Vec3::Cross(v1, v2);

	// 軸を計算
	// uとvが平行である場合、軸は任意のベクトルであるが、通常は(0, 0, 1)とする
	Vec3 axis = cross.Norm();

	// 軸回転を作成
	// ユニタリクォータニオンを作成し、acos関数を使用して角度を計算
	float theta = acosf(dot);

	// axisとthetaを使用して回転クォータニオンを作成
	return MakeAxisAngle(axis, theta);
}

Quaternion Quaternion::Lerp(const Quaternion s, const Quaternion e, const float f)
{
	Quaternion dis = e - s;
	return dis * f + s;
}

Vec3 Quaternion::RotateVector(const Quaternion& q, const Vec3& v)
{
	Quaternion p = { v.x, v.y, v.z, 0 };

	Quaternion result = q * p * q.Conjugate();

	return { result.x, result.y, result.z };
}

Quaternion Quaternion::operator-() const { return { -x,-y,-z,-w }; }

Quaternion Quaternion::operator+(const Quaternion other) const
{
	return { x + other.x, y + other.y, z + other.z, w + other.w };
}
Quaternion Quaternion::operator-(const Quaternion other) const
{
	return { x - other.x,y - other.y,z - other.z,w - other.w };
}
Quaternion Quaternion::operator*(const Quaternion other) const
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
Quaternion Quaternion::operator*(const float num) const
{
	return { x * num,y * num ,z * num ,w * num };
}
Quaternion Quaternion::operator/(const float num) const
{
	return { x / num,y / num ,z / num ,w / num };
}

Quaternion& Quaternion::operator+=(const Quaternion other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}
Quaternion& Quaternion::operator-=(const Quaternion other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}
Quaternion& Quaternion::operator*=(const Quaternion other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

Quaternion& Quaternion::operator*=(const float num)
{
	x *= num;
	y *= num;
	z *= num;
	w *= num;
	return *this;
}
Quaternion& Quaternion::operator/=(const float num)
{
	x /= num;
	y /= num;
	z /= num;
	w /= num;
	return *this;
}

Quaternion operator*(const float num, const Quaternion q)
{
	return { num * q.x,num * q.y ,num * q.z ,num * q.w };
}

