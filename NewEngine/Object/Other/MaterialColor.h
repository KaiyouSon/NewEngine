#pragma once
#include "Color.h"
#include <string>

struct MaterialColor
{
public:
	std::string name;
	Color ambient;		// �A���r�G���g
	Color diffuse;		// �f�B�t���[�Y
	Color specular;		// �X�y�L�����[
};
