#pragma once
#include "MathUtil.h"
#include "Easing.h"
#include "Color.h"

namespace ParticleParameter
{
	struct PParam0
	{
		Vec3 curPos;
		Vec2 curScale;
		float curRot;
		float curShininess = 1.0f;
		Color curColor;
	};

	struct PParam1
	{
		Vec3 moveVec;
		Vec3 moveAccel;

		Vec3 startPos;
		Vec3 endPos;

		Vec2 startScale;
		Vec2 endScale;

		float startRot;
		float endRot;

		float startShininess;
		float endShininess;

		Color startColor;
		Color endColor;

		Easing ease;
		PParam1();
	};

	struct ParticleMesh
	{
		Vec3 pos;
		Vec2 scale;
		float shininess;
		Color color;
	};

	struct GPUParticleTimer
	{
		uint32_t timer;
		uint32_t maxTimer;

		GPUParticleTimer();
		GPUParticleTimer(const uint32_t timer, const uint32_t maxTimer);
	};

	struct PonDeRing
	{
		Vec3 pos;
		Vec2 scale;
		float shininess;
		Color color;
		float timer;
		float moveSpeed;
	};

	struct LeadParticle
	{
		Vec3 pos;
		Vec3 moveVec;
		Vec3 moveAccel;
		Vec2 scale;
		float scaleAccel;
		float shininess;
		Color color;
	};

	struct PlayerRecoveryParticle
	{
		Vec3 pos;
		Vec3 moveVec;
		Vec3 moveAccel;
		Vec2 scale;
		float scaleAccel;
		float shininess;
		Color color;
		float alphaAccel;
	};

	struct AirParticle
	{
		Vec3 pos;
		Vec3 moveVec;
		Vec3 moveAccel;
		Vec2 scale;
		float shininess;
		Color color;
	};

	struct RespawnPointParticle
	{
		uint32_t isActive;
		Vec3 pos;
		Vec3 moveVec;
		Vec3 moveAccel;
		Vec2 scale;
		float shininess;
		Color color;

		RespawnPointParticle();
	};

	struct LogoExplosionParticle
	{
		Vec3 pos;
		Vec3 moveVec;
		Vec3 moveAccel;
		Vec2 scale;
		float shininess;
		Color color;
		float timer;
	};

	struct BossAttackTrajectoryParticle
	{
		Vec3 pos;
		Vec3 moveVec;
		Vec3 moveAccel;
		Vec2 scale;
		float shininess;
		Color color;
	};

	struct SmokeParticle
	{
		Vec3 pos;
		Vec3 moveVec;
		Vec3 moveAccel;
		Vec2 scale;
		Color color;
	};
}