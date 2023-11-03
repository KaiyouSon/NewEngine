#pragma once
#include <math.h>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "Viewport.h"
#include "RectAngle.h"

constexpr float PI = 3.141592f;

// 絶対値を計算
float Absolute(const float num);
Vec2 Absolute(const Vec2 num);
Vec3 Absolute(const Vec3 num);
// 弧度法から度数法へ変換
float Angle(const float radian);
Vec2 Angle(const Vec2 radian);
Vec3 Angle(const Vec3 radian);
// 度数法から弧度法へ変換
float Radian(const float angle);
Vec2 Radian(const Vec2 angle);
Vec3 Radian(const Vec3 angle);

// 3次元ベクトルと4x4行列の積
Vec3 Vec3MulMat4(const Vec3 v, const Mat4& m, const bool isMulW = false);

Mat4 ConvertScalingMat(Vec3 scale);		// 縮尺変換行列の作成
Mat4 ConvertRotationXAxisMat(float angle);	// x軸回転行列の作成
Mat4 ConvertRotationYAxisMat(float angle);	// y軸回転行列の作成
Mat4 ConvertRotationZAxisMat(float angle);	// z軸回転行列の作成
Mat4 ConvertTranslationMat(const Vec3 pos);	// 並進行列の作成

Mat4 ConvertRotationMat(const Quaternion q); // 回転行列の作成

// ビルボード処理
Mat4 ConvertBillBoardXAxis();
Mat4 ConvertBillBoardYAxis();
Mat4 ConvertBillBoardZAxis();
Mat4 ConvertBillBoardAllAxis();

Mat4 ConvertViewProjectionMatLookAt(const Vec3 pos, const Vec3 target, const Vec3 up);				// ビュー射影行列をLookAtから生成
Mat4 ConvertViewProjectionMatLookTo(const Vec3 pos, const Vec3 zAxis, const Vec3 yAxis);			// ビュー射影行列をLookToから生成
Mat4 ConvertPerspectiveProjectionMat(float fovAngle, float aspect, float nearZ, float farZ);		// 透視射影行列の生成
Mat4 ConvertOrthographicProjectionMat(float WIN_WIDTH, float WIN_HEIGHT);							// 正射影行列の生成
Mat4 ConvertOrthographicProjectionMat(const RectAngle rect, const float nearZ, const float farZ);	// 正射影行列の生成
Mat4 ConvertViewportMat(Viewport& viewport);

Mat4 CalculateWorldMat(const Vec3 pos, const Vec3 scale, const Vec3 rot);
Mat4 CalculateWorldMat(const Vec3 pos, const Vec3 scale, const Quaternion rot);

Vec3 operator+(const float num, const Vec3 v);
Vec3 operator-(const float num, const Vec3 v);
Vec3 operator*(const float num, const Vec3 v);
Vec3 operator/(const float num, const Vec3 v);

