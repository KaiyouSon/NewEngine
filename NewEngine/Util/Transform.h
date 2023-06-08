#pragma once
#include "Vec3.h"
#include "Mat4.h"
#include "Enum.h"

class Transform
{
public:
	Vec3 pos;	// 座標
	Vec3 scale;	// スケール
	Vec3 rot;	// 回転

private:
	Mat4 scaleMat_;	// スケール行列
	Mat4 rotMat_;	// 回転行列
	Mat4 transMat_;	// 平行移動行列
	Mat4 worldMat_;	// ワールド変換行列
	Mat4 billboardMat_; // ビルボード行列

	bool isUseBillboard_;
	BillBoardType billboardType_;

private:
	void BillBoardUpdate();

public:
	Transform();
	Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot);
	void Update();

public:	// セッター
	inline void SetisUseBillboard(const bool isUseBillboard) { isUseBillboard_ = isUseBillboard; }
	inline void SetBillboardType(const BillBoardType billboardType) { billboardType_ = billboardType; }

	inline bool GetisUseBillboard() { return isUseBillboard_; }
	inline BillBoardType GetBillboardType() { return billboardType_; }

#pragma region ゲッター

	Mat4 GetTransMat();
	Mat4 GetScaleMat();
	Mat4 GetRotMat();
	Mat4 GetWorldMat();

#pragma endregion

#pragma region セッター
	void SetWorldMat(Mat4 worldMat);

#pragma endregion
};

