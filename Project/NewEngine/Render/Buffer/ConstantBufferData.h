#pragma once
#include "Util.h"

namespace ConstantBufferData
{
	struct CTransformP
	{
		Mat4 viewMat;
		Mat4 worldMat;
		Mat4 billboardMat;
	};

	struct CTransform3D
	{
		Mat4 viewMat;
		Mat4 worldMat;
		Vec3 cameraPos;
	};

	struct CTransformCollider
	{
		Mat4 wvpMat;
	};

	struct CTransformShadowObj
	{
		Mat4 viewProjMat;
		Mat4 worldMat;
	};

	struct CTransform3DShadow
	{
		Mat4 viewProjMat;
		Mat4 shadowMat;
		Mat4 worldMat;
		Vec3 cameraPos;
		Vec3 lightCameraPos;
	};

	struct CTransformVolumetricFog
	{
		Mat4 viewMat;
		Mat4 worldMat;
		Mat4 rotateMat;
		Vec3 cameraPos;
	};

	struct CTransform2D
	{
		Mat4 mat;	//3D変換行列
	};

	struct CCircleGauge
	{
		float startRadian;
		float endRadian;
	};

	struct CUVParameter
	{
		Vec2 offset;
		Vec2 tiling;
	};

	struct CUVWParameter
	{
		Vec3 offset; float pad;
		Vec3 tiling;
	};

	struct CScreenToWorldMat
	{
		Mat4 screenToWorldMat;
	};

	struct CVolumetricFog
	{
		uint32_t stepCount;
		float stepLength;
		float dencity;		float pad;
		Color fogColor;
		Color fogColorRate;
		Vec2 fogClamp;
	};

	struct CObjectParam
	{
		Vec3 pos;	float pad;
		Vec3 scale;
	};

	struct CColor
	{
		Color color;// 色
	};

	struct CMaterialColor
	{
		Color ambient;// アンビエント係数
		Color diffuse;// ディフューズ係数
		Color specular;	// スペキュラー係数
	};

	struct CPOM
	{
		float heightScale;
	};

	struct CDirectionalLight
	{
		Color color;
		Vec3 vec;
		uint32_t isActive;
	};

	struct CPointLight
	{
		Color color;
		Vec3 pos;
		float radius;
		Vec3 colorRate;
		float decay;
		uint32_t isActive;	Vec3 pad;
	};

	struct CSpotLight
	{
		Color color;
		Vec3 vec;
		float radius;
		Vec3 pos;
		float decay;
		Vec3 colorRate;
		uint32_t isActive;
		Vec2 cosAngle; Vec2 pad;
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
		float focusDepth;
		float middleDepth;
		uint32_t isRGB;
	};

	struct CRadialBlur
	{
		Vec2 centerPos;
		float strength = 0.5f;
		float loopNum = 8.f;
	};

	struct CVignette
	{
		Color color;
		Vec2 range;
	};

	struct CDissolve
	{
		float dissolve;
		float colorPower;
		Vec2 pad;
		Color dissolveColor;
	};

	struct CShadowMap
	{
		uint32_t isWrite;
		float bias;
	};

	struct CRespawnTransition
	{
		float min;
		float max;
	};

	struct CLightView
	{
		Mat4 viewProjView;
		Vec3 cameraPos;
	};

	struct CTransformGrass
	{
		Mat4 viewProjMat;
		Mat4 shadowMat;
		Mat4 worldMat;
		Mat4 billboardMat;
		Vec3 cameraPos;
		Vec3 lightCameraPos;
	};

	struct CTextureSizeData
	{
		Vec2 size;
		float area;
	};

	struct CParticleObject
	{
		uint32_t vertexSize;
		uint32_t meshParticleSize;	// 1三角形にあるパーティクルの量
		uint32_t maxParticleSize;	// パーティクル全体の量
	};

	struct CMaxParticle
	{
		uint32_t max;
	};

	struct CPlayerRecoveryEffect
	{
		Vec3 baseSpeed;	float pad1;
		Vec3 rateSpeed; float pad2;
		Vec3 baseScale;	float pad3;
		Vec3 rateScale; float pad4;
		Vec3 baseScaleAccel; float pad5;
		Vec3 rateScaleAccel; float pad6;
		Color baseColor;
	};

	struct CBossAttackTrajectoryEffect
	{
		uint32_t isActive;
		Vec3 startPos;
		Vec3 endPos;
	};

	struct CSmokeEffect
	{
		uint32_t isGenerate;
		Vec3 generatePos;
		uint32_t generateTimer;
		Vec3 generateRange;
		uint32_t generateNum;
	};
}
