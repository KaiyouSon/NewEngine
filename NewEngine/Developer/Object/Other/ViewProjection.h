#pragma once
#include "MathUtil.h"
#include "GameObject.h"
#include "Singleton.h"

template<typename T> class Singleton;

class ViewProjection : public Singleton<ViewProjection> //: public GameObject
{
private:
	friend Singleton<ViewProjection>;
	Vec3 pos = { 0,0,-30 };		// ���_���W
	Vec3 target = { 0,0,0 };	// �����_���W
	Vec3 up = { 0,1,0 };		// ������x�N�g��
	float nearZ;
	float farZ;

public:
	Mat4 matView;			// �r���[�ϊ��s��
	Mat4 matProjection2D;	// ���s���e�s��
	Mat4 matProjection3D;	// �������e�s��

public:
	void Initialize();
	void Update();

public:	// �Z�b�^�[
	void SetPos(const Vec3& pos) { this->pos = pos; }
	void SetTarget(const Vec3& target) { this->target = target; }
	void SetUp(const Vec3& up) { this->up = up; }

public:	// �Q�b�^�[
	inline Vec3 GetPos() { return pos; }
	inline Vec3 GetTarget() { return target; }
	inline Vec3 GetUp() { return up; }
	inline float GetNearZ() { return nearZ; }
	inline float GetFarZ() { return farZ; }

}typedef View;

extern View* view;