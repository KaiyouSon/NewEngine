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

Quaternion Quaternion::AnyAxisRotation(const Vec3 v, const float radian)
{
	Quaternion result = { x,y,z,w };

	// 蝗櫁ｻ｢繧ｯ繧ｩ繝ｼ繧ｿ繝九が繝ｳ菴懈・
	Quaternion q =
	{
		v.Norm().x * sinf(radian / 2),
		v.Norm().y * sinf(radian / 2),
		v.Norm().z * sinf(radian / 2),
		cosf(radian / 2)
	};
	// 蝗櫁ｻ｢繧ｯ繧ｩ繝ｼ繧ｿ繝九が繝ｳ縺ｮ蜈ｱ蠖ｹ繧ｯ繧ｩ繝ｼ繧ｿ繝九が繝ｳ菴懈・      
	Quaternion qc = q.Conjugate();

	// 蝗櫁ｻ｢蜃ｦ逅・
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

Quaternion Quaternion::MakeAxisAngle(const Vec3 v, const float radian)
{
	// 蝗櫁ｻ｢繧ｯ繧ｩ繝ｼ繧ｿ繝九が繝ｳ菴懈・
	Quaternion q =
	{
		v.Norm().x * sinf(radian / 2),
		v.Norm().y * sinf(radian / 2),
		v.Norm().z * sinf(radian / 2),
		cosf(radian / 2)
	};

	return q;
}

Quaternion Quaternion::DirectionToDirection(const Vec3 v1, const Vec3 v2)
{
	// u縺ｨv繧呈ｭ｣隕丞喧縺励※蜀・ｩ阪ｒ豎ゅａ繧九Ｖ,v繧貞腰菴阪・繧ｯ繝医Ν蜑肴署縺ｨ縺吶ｋ縺ｪ繧画ｭ｣隕丞喧縺ｯ荳崎ｦ・
	Vec3 u = v1;
	u.Norm();
	Vec3 v = v2;
	v.Norm();

	float dot = Vec3::Dot(u.Norm(), v.Norm());

	// u,v縺ｮ螟也ｩ阪ｒ縺ｨ繧・
	Vec3 cross = Vec3::Cross(v1, v2);

	// 霆ｸ縺ｯ蜊倅ｽ阪・繧ｯ繝医Ν縺ｧ縺ゅｋ蠢・ｦ√′縺ゅｋ縺ｮ縺ｧ豁｣隕丞喧
	// u縺ｨv縺悟腰菴阪・繧ｯ繝医Ν縺ｧ縺ゅ▲縺ｦ繧ゅ∝､也ｩ阪′蜊倅ｽ阪・繧ｯ繝医Ν縺ｨ縺ｯ髯舌ｉ縺ｪ縺・・縺ｧ縺薙％縺ｮ豁｣隕丞喧縺ｯ蠢・・
	Vec3 axis = cross.Norm();

	// 蜊倅ｽ阪・繧ｯ繝医Ν縺ｧ蜀・ｩ阪ｒ縺ｨ縺｣縺ｦ縺・ｋ縺ｮ縺ｧacos縺ｧ隗貞ｺｦ繧呈ｱゅａ繧・
	float theta = acosf(dot);

	// axis縺ｨtheta縺ｧ莉ｻ諢剰ｻｸ蝗櫁ｻ｢繧剃ｽ懊▲縺ｦ霑斐☆
	return MakeAxisAngle(axis, theta);
}

Quaternion Quaternion::Lerp(const Quaternion s, const Quaternion e, const float f)
{
	Quaternion dis = e - s;
	return dis * f + s;
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

