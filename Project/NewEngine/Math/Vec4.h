#pragma once
struct Vec2;

struct Vec4
{
public:
	float x, y, z, w;

public:
	Vec4& operator=(const Vec2 v);
	Vec4& operator=(const Vec3 v);
	Vec4& operator=(const Vec4 v);
};

