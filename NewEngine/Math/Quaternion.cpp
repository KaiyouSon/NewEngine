#include "Quaternion.h"
#include "MathUtil.h"

Quaternion Quaternion::Conjugate()
{
	return { -x,-y,-z,w };
}

Quaternion Quaternion::AnyAxisRotation(const Vec3& v, const float& angle)
{
	Quaternion result = { x,y,z,w };

	// XY平面上の軸で180°回転クォータニオン作成      
	Quaternion q = Quaternion::MakeAxisRotation(v, Radian(angle));
	// 回転クォータニオンの共役クォータニオン作成      
	Quaternion qc = q.Conjugate();

	// 回転処理
	result = q * result;
	result = result * qc;

	return result;
}

Quaternion Quaternion::MakeAxisRotation(const Vec3& v, const float& angle)
{
	float sinA = sinf(angle / 2.0f);
	return { v.Norm().x * sinA, v.Norm().y * sinA, v.Norm().z * sinA, cosf(angle / 2.0f) };
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	Quaternion result;
	float num[4] = {};

	num[0] = +w * q.w;
	num[1] = -x * q.x;
	num[2] = -y * q.y;
	num[3] = -z * q.z;
	result.w = num[0] + num[1] + num[2] + num[3];

	num[0] = +w * q.x;
	num[1] = +q.w * x;
	num[2] = +y * q.z;
	num[3] = -z * q.y;
	result.x = num[0] + num[1] + num[2] + num[3];

	num[0] = +w * q.y;
	num[1] = +q.w * y;
	num[2] = +z * q.x;
	num[3] = -x * q.z;
	result.y = num[0] + num[1] + num[2] + num[3];

	num[0] = +w * q.z;
	num[1] = +q.w * z;
	num[2] = +x * q.y;
	num[3] = -y * q.x;
	result.z = num[0] + num[1] + num[2] + num[3];

	return result;
}
