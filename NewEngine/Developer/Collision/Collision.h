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
	// ���Ƌ�
	bool SphereHitSphere(
		const SphereCollider& sphere1,
		const SphereCollider& sphere2);

	// ���C�ƃ��b�V��
	bool ReyHitMesh(
		const ReyCollider& rey,
		const MeshCollider& mesh);

	// ���ƃ��b�V��
	bool LineHitMesh(
		const LineCollider& line,
		const MeshCollider& mesh);

public:
	// ����������
	//bool OnTrigger(
	//	const GameObject* obj1,
	//	const GameObject* obj2);
};