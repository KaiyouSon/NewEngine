#pragma once
#include "Util.h"

namespace ConstantBufferData
{
	struct CTransform3D
	{
		Mat4 viewMat;
		Mat4 worldMat;
		Vec3 cameraPos;
	};

	struct CTransform2D
	{
		Mat4 mat;	//3D�ϊ��s��
	};

	struct CCircleGauge
	{
		float startRadian;
		float endRadian;
	};

	struct CColor
	{
		Color color;// �F
	};

	struct CMaterialColor
	{
		Color ambient;// �A���r�G���g�W��
		Color diffuse;// �f�B�t���[�Y�W��
		Color specular;	// �X�y�L�����[�W��
	};

	struct CDirectionalLight
	{
		Vec3 dir;		float pad1;
		Vec3 color;
		uint32_t isActive;
	};

	struct CPointLight
	{
		Vec3 pos;		float pad1;
		Vec3 color;		float pad2;
		Vec3 atten;
		uint32_t isActive;
	};

	struct CSpotLight
	{
		Vec3 vec;		float pad1;
		Vec3 pos;		float pad2;
		Vec3 color;		float pad3;
		Vec3 atten;		float pad4;
		Vec2 factorAngleCos;
		uint32_t isActive;	float pad5;
	};

	struct CCircleShadow
	{
		Vec3 vec;		float pad1;
		Vec3 pos;
		float disCasterLight;
		Vec3 atten;		float pad2;
		Vec2 factorAngleCos;
		uint32_t isActive;	float pad3;
	};

	struct CFog
	{
		uint32_t isActive;
		float nearDis;
		float farDis;	float pad;
		Color color;
	};

	static const int maxBones = 32;
	struct CSkin
	{
		Mat4 bones[maxBones];
	};

	struct CLineBlur
	{
		float angle;
	};

	struct CDOF
	{
		float focusWidth;
		float focusDepth;
	};
}
