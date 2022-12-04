#pragma once
#include "ColliderManager.h"
#include "Singleton.h"
#include <list>
#include <memory>

template<typename T> class Singleton;

class Collision : public Singleton<Collision>
{
private:
	friend Singleton<Collision>;

private:
	// 球と球
	bool SphereHitSphere(
		const SphereCollider& sphere1,
		const SphereCollider& sphere2);

	// レイとメッシュ
	bool ReyHitMesh(
		const ReyCollider& rey,
		const MeshCollider& mesh);

	// 線とメッシュ
	bool LineHitMesh(
		const LineCollider& line,
		const MeshCollider& mesh);

public:
	// 当たったら
	//bool OnTrigger(
	//	const GameObject* obj1,
	//	const GameObject* obj2);
};