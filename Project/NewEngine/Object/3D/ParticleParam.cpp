#include "ParticleParam.h"
using namespace ParticleParameter;

PParam1::PParam1() :
	moveVec(0), moveAccel(0),
	startScale(1), endScale(1),
	startRot(0), endRot(0)
{
}

GPUParticleTimer::GPUParticleTimer() : timer(0), maxTimer(0)
{
}
GPUParticleTimer::GPUParticleTimer(const uint32_t timer, const uint32_t maxTimer) :
	timer(timer), maxTimer(maxTimer)
{
}

RespawnPointParticle::RespawnPointParticle() :
	isActive(false),
	pos(0), moveVec(0), moveAccel(0),
	scale(0), shininess(0), color(Color::one)
{
}
