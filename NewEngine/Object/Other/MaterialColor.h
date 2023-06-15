#pragma once
#include "Color.h"
#include <string>

struct MaterialColor
{
public:
	std::string name;
	Color ambient;		// アンビエント
	Color diffuse;		// ディフューズ
	Color specular;		// スペキュラー
};
