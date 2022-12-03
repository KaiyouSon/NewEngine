#pragma once
#include "IComponent.h"
#include "MathUtil.h"

enum BillBoardType
{
	XAxisBillboard,
	YAxisBillboard,
	ZAxisBillboard,
	AllAxisBillboard,
};

class Transform : public IComponent
{
public:
	Vec3 pos;	// 座標
	Vec3 scale;	// スケール
	Vec3 rot;	// 回転

	// ワールド変換行列
	Mat4 worldMat;
private:
	Mat4 scaleMat;	// スケール行列
	Mat4 rotMat;	// 回転行列
	Mat4 transMat;	// 平行移動行列
	Mat4 billboardMat;

	bool isUseBillboard;
	int billboardType;

private:
	void BillBoardUpdate();

public:
	Transform();
	Transform(Vec3 pos, Vec3 scale, Vec3 rot);
	void Update();

public:	// セッター
	inline void SetisUseBillboard(const bool& isUseBillboard) { this->isUseBillboard = isUseBillboard; }
	inline void SetBillboardType(const int& billboardType) { this->billboardType = billboardType; }

	inline bool GetisUseBillboard() { return isUseBillboard; }
	inline int GetBillboardType() { return billboardType; }

};

