#pragma once
#include "MathUtil.h"
#include "Util.h"

template<typename T> class Singleton;

class DebugCamera : public Singleton<DebugCamera>
{
private:
	friend Singleton<DebugCamera>;

	Vec3 pos;		// 視点座標
	Vec3 target;	// 注視点座標
	Vec3 up;		// 上方向ベクトル

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