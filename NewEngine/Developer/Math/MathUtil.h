#pragma once
#include <math.h>
#include "Singleton.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Viewport.h"

template<typename T> class Singleton;

class MathUtil : public Singleton<MathUtil>
{
private:
	friend Singleton<MathUtil>;
	MathUtil() = default;
	~MathUtil() = default;

public:
	const float PI = 3.141592f;

	// 絶対値を返す関数
	inline float Absolut(const float& num) { return num >= 0 ? num : num * -1; }

	// 弧度法から度数法に変換する関数
	inline float Angle(const float& radian) { return radian * 360 / (2 * PI); }
	inline Vec2 Angle(const Vec2& radian) { return { Angle(radian.x),Angle(radian.y) }; }
	inline Vec3 Angle(const Vec3& radian) { return { Angle(radian.x),Angle(radian.y),Angle(radian.z) }; }
	// 度数法から弧度法に変換する関数
	inline float Radian(const float& angle) { return angle * (2 * PI) / 360; }
	inline Vec2 Radian(const Vec2& angle) { return { Radian(angle.x),Radian(angle.y) }; }
	inline Vec3 Radian(const Vec3& angle) { return { Radian(angle.x),Radian(angle.y),Radian(angle.z) }; }

	Mat4 ConvertScalingMat(Vec3 scale);		// スケーリング行列を返す関数
	Mat4 ConvertRotationXAxisMat(float angle);	// 回転行列ｘ軸を返す関数
	Mat4 ConvertRotationYAxisMat(float angle);	// 回転行列ｙ軸を返す関数
	Mat4 ConvertRotationZAxisMat(float angle);	// 回転行列ｚ軸を返す関数
	Mat4 ConvertTranslationMat(const Vec3& pos);	// 平行移動を返す関数

	 // ビルボード行列変換
	Mat4 ConvertBillBoardXAxis();
	Mat4 ConvertBillBoardYAxis();
	Mat4 ConvertBillBoardZAxis();
	Mat4 ConvertBillBoardAllAxis();

	Mat4 ConvertViewProjectionMat(const Vec3& pos, const Vec3& target, const Vec3& up);			// ビュー変換
	Mat4 ConvertPerspectiveProjectionMat(float fovAngle, float aspect, float nearZ, float farZ);	// 透視射影変換
	Mat4 ConvertOrthoGrphicProjectionMat(int WIN_WIDTH, int WIN_HEIGHT);							// 平行射影変換
	Mat4 ConvertViewportMat(Viewport& viewport);
};

extern MathUtil* mathUtil;