#pragma once
#include <math.h>
#include <fbxsdk.h>
#include "Vec2.h"
#include "Vec3.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "Viewport.h"

constexpr float PI = 3.141592f;

// 絶対値を返す関数
inline float Absolut(const float num) { return num >= 0 ? num : num * -1; }
inline Vec2 Absolut(const Vec2 num) { return { Absolut(num.x),Absolut(num.y) }; }
inline Vec3 Absolut(const Vec3 num) { return { Absolut(num.x),Absolut(num.y),Absolut(num.z) }; }
// 弧度法を度数法に変換する関数
inline float Angle(const float radian) { return radian * 360 / (2 * PI); }
inline Vec2 Angle(const Vec2 radian) { return { Angle(radian.x),Angle(radian.y) }; }
inline Vec3 Angle(const Vec3 radian) { return { Angle(radian.x),Angle(radian.y),Angle(radian.z) }; }
// 度数法から弧度法に変換する関数
inline float Radian(const float angle) { return angle * (2 * PI) / 360; }
inline Vec2 Radian(const Vec2 angle) { return { Radian(angle.x),Radian(angle.y) }; }
inline Vec3 Radian(const Vec3 angle) { return { Radian(angle.x),Radian(angle.y),Radian(angle.z) }; }

// 3次元ベクトルと4x4行列の掛け算
Vec3 Vec3MulMat4(const Vec3 v, const Mat4& m, const bool isMulW = false);
void ConvertMat4FromFbx(Mat4* dst, const FbxAMatrix& src);

Mat4 ConvertScalingMat(Vec3 scale);		// スケーリング行列を返す関数
Mat4 ConvertRotationXAxisMat(float angle);	// 回転行列ｘ軸を返す関数
Mat4 ConvertRotationYAxisMat(float angle);	// 回転行列ｙ軸を返す関数
Mat4 ConvertRotationZAxisMat(float angle);	// 回転行列ｚ軸を返す関数
Mat4 ConvertTranslationMat(const Vec3 pos);	// 平行移動を返す関数

Mat4 ConvertRotationMat(const Quaternion q); // クォータニオンで回転行列を生成する関数

// ビルボード行列変換
Mat4 ConvertBillBoardXAxis();
Mat4 ConvertBillBoardYAxis();
Mat4 ConvertBillBoardZAxis();
Mat4 ConvertBillBoardAllAxis();

Mat4 ConvertViewProjectionMatLookAt(const Vec3 pos, const Vec3 target, const Vec3 up);		// ビュー変換
Mat4 ConvertViewProjectionMatLookTo(const Vec3 pos, const Vec3 zAxis, const Vec3 yAxis);		// ビュー変換
Mat4 ConvertPerspectiveProjectionMat(float fovAngle, float aspect, float nearZ, float farZ);	// 透視射影変換
Mat4 ConvertOrthoGrphicProjectionMat(float WIN_WIDTH, float WIN_HEIGHT);						// 平行射影変換
Mat4 ConvertViewportMat(Viewport& viewport);

Mat4 CalculateWorldMat(const Vec3 pos, const Vec3 scale, const Vec3 rot);
Mat4 CalculateWorldMat(const Vec3 pos, const Vec3 scale, const Quaternion rot);

Vec3 operator+(const float num, const Vec3 v);
Vec3 operator-(const float num, const Vec3 v);
Vec3 operator*(const float num, const Vec3 v);
Vec3 operator/(const float num, const Vec3 v);
