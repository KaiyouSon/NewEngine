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
	Vec3 ambient;		// �A���r�G���g
	Vec3 diffuse;		// �f�B�t���[�Y
	Vec3 specular;		// �X�y�L�����[
	float alpha;		// �A���t�@
	Texture texture;

public:
	Material();
	Material(std::string filePath);
};