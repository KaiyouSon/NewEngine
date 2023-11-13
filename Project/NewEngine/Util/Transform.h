#pragma once
#include "Mat4.h"
#include "Billboard.h"

// トランスフォームクラス
class Transform
{
private:
	Mat4 mScaleMat;  // スケール行列
	Mat4 mRotMat;    // 回転行列
	Mat4 mTransMat;  // 位置変換行列
	Mat4 mWorldMat;  // ワールド行列

	Billboard mBillboard;

public:
	Vec3 pos;     // 位置
	Vec3 scale;   // スケール
	Vec3 rot;     // 回転

public:
	Transform();
	Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot);
	void Update();

public: // 行列取得
	Mat4 GetTransMat();
	Mat4 GetScaleMat();
	Mat4 GetRotMat();
	Mat4 GetWorldMat();

public: // ワールド行列設定
	void SetWorldMat(Mat4 worldMat);
	void SetBillboardType(const BillboardType type);
};