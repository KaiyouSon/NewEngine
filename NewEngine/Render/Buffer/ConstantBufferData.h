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

struct ConstantBufferDataSpotLight
{
	Vec3 vec;
	float pad1;
	Vec3 pos;
	float pad2;
	Vec3 color;
	float pad3;
	Vec3 atten;
	float pad4;
	Vec2 factorAngleCos;
	unsigned int isActive;
	float pad5;
};

struct ConstantBufferDataCircleShadow
{
	Vec3 vec;
	float pad1;
	Vec3 pos;
	float disCasterLight;
	Vec3 atten;
	float pad2;
	Vec2 factorAngleCos;
	unsigned int isActive;
	float pad3;
};

struct ConstantBufferDataFog
{
	unsigned int isActive;
	float nearDis;
	float farDis;
	float pad;
	Color color;
};