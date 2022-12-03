#pragma once
#include "MathUtil.h"
#include "GameObject.h"
#include "Singleton.h"

template<typename T> class Singleton;

class ViewProjection : public Singleton<ViewProjection> //: public GameObject
{
private:
	friend Singleton<ViewProjection>;
	Vec3 pos = { 0,0,-30 };		// 視点座標
	Vec3 target = { 0,0,0 };	// 注視点座標
	Vec3 up = { 0,1,0 };		// 上方向ベクトル
	float nearZ;
	float farZ;

public:
	Mat4 matView;			// ビュー変換行列
	Mat4 matProjection2D;	// 平行投影行列
	Mat4 matProjection3D;	// 透視投影行列

public:
	void Initialize();
	void Update();

public:	// セッター
	void SetPos(const Vec3& pos) { this->pos = pos; }
	void SetTarget(const Vec3& target) { this->target = target; }
	void SetUp(const Vec3& up) { this->up = up; }

public:	// ゲッター
	inline Vec3 GetPos() { return pos; }
	inline Vec3 GetTarget() { return target; }
	inline Vec3 GetUp() { return up; }
	inline float GetNearZ() { return nearZ; }
	inline float GetFarZ() { return farZ; }

}typedef View;

extern View* view;