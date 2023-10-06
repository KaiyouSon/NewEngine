#include "Transform.h"
#include "MathUtil.h"

Transform::Transform() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	mWorldMat(Mat4::Identity()), mScaleMat(Mat4::Identity()),
	mRotMat(Mat4::Identity()), mTransMat(Mat4::Identity())
{
}

Transform::Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot) :
	pos(pos), scale(scale), rot(rot),
	mWorldMat(Mat4::Identity()), mScaleMat(Mat4::Identity()),
	mRotMat(Mat4::Identity()), mTransMat(Mat4::Identity())
{
}

void Transform::Update()
{
	mScaleMat = ConvertScalingMat(scale); // 繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ
	mRotMat = Mat4::Identity();
	mRotMat *= ConvertRotationZAxisMat(rot.z); // z霆ｸ蝗櫁ｻ｢
	mRotMat *= ConvertRotationXAxisMat(rot.x); // x霆ｸ蝗櫁ｻ｢
	mRotMat *= ConvertRotationYAxisMat(rot.y); // y霆ｸ蝗櫁ｻ｢
	mTransMat = ConvertTranslationMat(pos);	 // 蟷ｳ陦檎ｧｻ蜍・

	// 繝薙Ν繝懊・繝芽｡悟・險育ｮ・
	mBillboard.CalculateBillboardMat();

	// 繝ｯ繝ｼ繝ｫ繝芽｡悟・險育ｮ・
	mWorldMat = Mat4::Identity();

	if (mBillboard.GetBillboardType() != BillboardType::None)
	{
		mWorldMat *= mBillboard.GetMat();
	}

	mWorldMat *= mScaleMat;
	mWorldMat *= mRotMat;
	mWorldMat *= mTransMat;
}

// 繧ｲ繝・ち繝ｼ
Mat4 Transform::GetTransMat() { return mTransMat; }	// 遘ｻ蜍戊｡悟・
Mat4 Transform::GetScaleMat() { return mScaleMat; }	// 繧ｹ繧ｱ繝ｼ繝ｫ陦悟・
Mat4 Transform::GetRotMat() { return mRotMat; }		// 蝗櫁ｻ｢陦悟・
Mat4 Transform::GetWorldMat() { return mWorldMat; }	// 繝ｯ繝ｼ繝ｫ繝芽｡悟・

// 繧ｻ繝・ち繝ｼ
void Transform::SetWorldMat(Mat4 worldMat) { mWorldMat = worldMat; }
void Transform::SetBillboardType(const BillboardType type) { mBillboard.SetBillboardType(type); }
