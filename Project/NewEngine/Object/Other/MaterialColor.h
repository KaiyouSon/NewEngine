#pragma once
#include "Color.h"
#include <string>

struct MaterialColor
{
public:
	Color ambient;		// アンビエント
	Color diffuse;		// ディフューズ
	Color specular;		// スペキュラー
	std::string name;
};

