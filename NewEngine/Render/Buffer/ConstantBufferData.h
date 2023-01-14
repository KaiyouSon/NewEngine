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