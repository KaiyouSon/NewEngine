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
	Mat4 mat;	//3D�ϊ��s��
};

struct ConstantBufferDataCircleGauge
{
	float startRadian;
	float endRadian;
	Vec2 pad;
};

struct ConstantBufferDataColor
{
	Color color;// �F
};

struct ConstantBufferDataMaterial
{
	Vec3 ambient;	// �A���r�G���g�W��
	float pad1;
	Vec3 diffuse;	// �f�B�t���[�Y�W��
	float pad2;
	Vec3 specular;	// �X�y�L�����[�W��
	float alpha;	// �A���t�@
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

static const int maxBones = 32;
struct ConstantBufferDataSkin
{
	Mat4 bones[maxBones];
};