#pragma once
#include "Util.h"
#include "Vec3.h"
#include "Mat4.h"

enum BillBoardType
{
	XAxisBillboard,
	YAxisBillboard,
	ZAxisBillboard,
	AllAxisBillboard,
};

class Transform
{
public:
	Vec3 pos;	// 座標
	Vec3 scale;	// スケール
	Vec3 rot;	// 回転
	Mat4 worldMat_;	// ワールド変換行列

private:
	Mat4 scaleMat;	// スケール行列
	Mat4 rotMat;	// 回転行列
	Mat4 transMat;	// 平行移動行列
	Mat4 billboardMat; // ビルボード行列

	bool isUseBillboard;
	int billboardType;

private:
	void BillBoardUpdate();

public:
	Transform();
	Transform(const Vec3& pos, const Vec3& scale, const Vec3& rot);
	void Update();

public:	// セッター
	inline void SetisUseBillboard(const bool& isUseBillboard) { this->isUseBillboard = isUseBillboard; }
	inline void SetBillboardType(const int& billboardType) { this->billboardType = billboardType; }

	inline bool GetisUseBillboard() { return isUseBillboard; }
	inline int GetBillboardType() { return billboardType; }

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

