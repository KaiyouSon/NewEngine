#include "stdafx.h"
#include "Vec4.h"

Vec4& Vec4::operator=(const Vec2 v)
{
	x = v.x;	y = v.y;
	return *this;
}

Vec4& Vec4::operator=(const Vec3 v)
{
	x = v.x;	y = v.y;	z = v.z;
	return *this;
}

Vec4& Vec4::operator=(const Vec4 v)
{
	x = v.x;	y = v.y;	z = v.z;	w = v.w;
	return *this;
}
