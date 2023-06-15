#pragma once
#include "MathUtil.h"
#include "Texture.h"
#include <string>
#include <memory>
#include <list>

struct MaterialColor
{
public:
	std::string name;
	Color ambient;		// アンビエント
	Color diffuse;		// ディフューズ
	Color specular;		// スペキュラー
	Texture texture;

//public:
//	MaterialColor();
//	MaterialColor(std::string filePath);
};

//class Material
//{
//public:
//	std::string name;
//	Color ambient;		// アンビエント
//	Color diffuse;		// ディフューズ
//	Color specular;		// スペキュラー
//	float alpha;		// アルファ
//	Texture texture;
//
//public:
//	//Material();
//	//Material(std::string filePath);
//};