#pragma once
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObject.h"
#include "Util.h"
#include <vector>
#include <forward_list>
#include <list>
#include <memory>

template<typename T>
struct RandomParam
{
	bool isRandom = false;
	T minValue = {};
	T maxValue = {};
};

template<typename T>
struct PeriodParam
{
	T start;
	T end;
	T now;

	RandomParam<T> srp;
	RandomParam<T> erp;
};

struct Particle
{
	Vec3 pos;	// ���W
	Vec3 speed;	// �X�s�[�h
	Vec3 addSpeed;	// �X�s�[�h

	PeriodParam<Vec3> vec;		// �x�N�g��
	PeriodParam<Color> color;	// �F
	PeriodParam<float> scale;	// �X�P�[��

	int activeTimer = 0;
	int activeMaxTimer = 60;
};

class Emitter : public GameObject
{
private:
	VertexBuffer<VertexParticle>* vertexBuffer;
	ConstantBuffer* constantBuffer;
	std::vector<VertexParticle> vertices;

public:
	std::forward_list<Particle> particles;
	Particle initialParticle;
	int maxParticle;
	bool isGenerate;

private:
	void ParticleDataInit();
	void ParticleGenerate();
	void ParticleUpdate();

public:
	Emitter();
	~Emitter();
	void Initialize();
	void Update();
	void Draw();

};

