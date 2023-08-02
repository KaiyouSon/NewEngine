#pragma once
#include "Vec3.h"
#include "Billboard.h"

class Transform
{
private:
	Mat4 mScaleMat;	// スケール行列
	Mat4 mRotMat;	// 回転行列
	Mat4 mTransMat;	// 平行移動行列
	Mat4 mWorldMat;	// ワールド変換行列

	Billboard mBillboard;

public:
	Vec3 pos;	// 座標
	Vec3 scale;	// スケール
	Vec3 rot;	// 回転

public:
	Transform();
	Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot);
	void Update();

public: // ゲッター
	Mat4 GetTransMat();
	Mat4 GetScaleMat();
	Mat4 GetRotMat();
	Mat4 GetWorldMat();

public: // セッター
	void SetWorldMat(Mat4 worldMat);
	void SetBillboardType(const BillboardType type);
};

