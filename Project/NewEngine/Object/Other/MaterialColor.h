#pragma once
#include "Color.h"
#include <string>

struct MaterialColor
{
public:
	Color ambient;		// 繧｢繝ｳ繝薙お繝ｳ繝・
	Color diffuse;		// 繝・ぅ繝輔Η繝ｼ繧ｺ
	Color specular;		// 繧ｹ繝壹く繝･繝ｩ繝ｼ
	std::string name;
};

