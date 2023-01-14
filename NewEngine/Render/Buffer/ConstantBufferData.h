#pragma once
#include "Util.h"

struct ConstantBufferDataTransform3D
{
	Mat4 viewMat;
	Mat4 worldMat;
	Vec3 cameraPos;
};

struct ConstantBufferDataTransform2D
{
	Mat4 mat;	//3D変換行列
};

struct ConstantBufferDataColor
{
	Color color;// 色
};

struct ConstantBufferDataMaterial
{
	Vec3 ambient;	// アンビエント係数
	float pad1;
	Vec3 diffuse;	// ディフューズ係数
	float pad2;
	Vec3 specular;	// スペキュラー係数
	float alpha;	// アルファ
};

struct ConstantBufferDataDirectionalLight
{
	Vec3 dir;
	float pad1;
	Vec3 color;
	unsigned int isActive;
};

struct ConstantBufferDataPointLight
{
	Vec3 pos;
	float pad1;
	Vec3 color;
	float pad2;
	Vec3 atten;
	unsigned int isActive;
};