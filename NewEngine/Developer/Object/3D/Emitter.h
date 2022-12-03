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
	Vec3 pos;	// 座標
	Vec3 speed;	// スピード
	Vec3 addSpeed;	// スピード

	PeriodParam<Vec3> vec;		// ベクトル
	PeriodParam<Color> color;	// 色
	PeriodParam<float> scale;	// スケール

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

