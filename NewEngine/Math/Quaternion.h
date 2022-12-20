#pragma once
#include "Vec3.h"

class Vec3;

class Quaternion
{
public:
	float x, y, z, w;

	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(const float& x, const float& y, const float& z) : x(x), y(y), z(z), w(0) {}
	Quaternion(const float& x, const float& y, const float& z, const float& w) : x(x), y(y), z(z), w(w) {}


	Quaternion Conjugate();
	Quaternion AnyAxisRotation(const Vec3& v, const float& angle);

	static Quaternion MakeAxisRotation(const Vec3& v, const float& angle);

	Quaternion operator*(const Quaternion& q) const;	// ˆê‚Â‚Ì’l‚Æ‚ÌŠ|‚¯ŽZ
};

