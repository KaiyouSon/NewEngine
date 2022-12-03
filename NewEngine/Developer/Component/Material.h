#pragma once
#include "MathUtil.h"
#include "Texture.h"
#include <string>
#include <memory>
#include <list>

class Material
{
public:
	std::string name;
	Vec3 ambient;		// アンビエント
	Vec3 diffuse;		// ディフューズ
	Vec3 specular;		// スペキュラー
	float alpha;		// アルファ
	Texture texture;

public:
	Material();
	Material(std::string filePath);
};