#pragma once
#include "MathUtil.h"
#include "Util.h"

template<typename T> class Singleton;

class DebugCamera : public Singleton<DebugCamera>
{
private:
	friend Singleton<DebugCamera>;

	Vec3 pos;		// ���_���W
	Vec3 target;	// �����_���W
	Vec3 up;		// ������x�N�g��

	Vec2 clickedVec;
	Vec2 oldClickedVec;
	Vec3 velocity;

public:
	void Initialize();
	void Update();

	inline Vec3 GetPos() { return pos; }
	inline Vec3 GetTarget() { return target; }
	inline Vec3 GetUp() { return up; }
};

extern DebugCamera* debugCamera;