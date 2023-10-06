#pragma once
#include "Vec3.h"
#include "Billboard.h"

class Transform
{
private:
	Mat4 mScaleMat;	// 繧ｹ繧ｱ繝ｼ繝ｫ陦悟・
	Mat4 mRotMat;	// 蝗櫁ｻ｢陦悟・
	Mat4 mTransMat;	// 蟷ｳ陦檎ｧｻ蜍戊｡悟・
	Mat4 mWorldMat;	// 繝ｯ繝ｼ繝ｫ繝牙､画鋤陦悟・

	Billboard mBillboard;

public:
	Vec3 pos;	// 蠎ｧ讓・
	Vec3 scale;	// 繧ｹ繧ｱ繝ｼ繝ｫ
	Vec3 rot;	// 蝗櫁ｻ｢

public:
	Transform();
	Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot);
	void Update();

public: // 繧ｲ繝・ち繝ｼ
	Mat4 GetTransMat();
	Mat4 GetScaleMat();
	Mat4 GetRotMat();
	Mat4 GetWorldMat();

public: // 繧ｻ繝・ち繝ｼ
	void SetWorldMat(Mat4 worldMat);
	void SetBillboardType(const BillboardType type);
};
