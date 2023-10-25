#pragma once
#include <math.h>
#include "Vec2.h"
#include "Vec3.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "Viewport.h"
#include "RectAngle.h"

constexpr float PI = 3.141592f;

// 邨ｶ蟇ｾ蛟､繧定ｿ斐☆髢｢謨ｰ
inline float Absolut(const float num) { return num >= 0 ? num : num * -1; }
inline Vec2 Absolut(const Vec2 num) { return { Absolut(num.x),Absolut(num.y) }; }
inline Vec3 Absolut(const Vec3 num) { return { Absolut(num.x),Absolut(num.y),Absolut(num.z) }; }
// 蠑ｧ蠎ｦ豕輔ｒ蠎ｦ謨ｰ豕輔↓螟画鋤縺吶ｋ髢｢謨ｰ
inline float Angle(const float radian) { return radian * 180 / PI; }
inline Vec2 Angle(const Vec2 radian) { return { Angle(radian.x),Angle(radian.y) }; }
inline Vec3 Angle(const Vec3 radian) { return { Angle(radian.x),Angle(radian.y),Angle(radian.z) }; }
// 蠎ｦ謨ｰ豕輔°繧牙ｼｧ蠎ｦ豕輔↓螟画鋤縺吶ｋ髢｢謨ｰ
inline float Radian(const float angle) { return angle * PI / 180; }
inline Vec2 Radian(const Vec2 angle) { return { Radian(angle.x),Radian(angle.y) }; }
inline Vec3 Radian(const Vec3 angle) { return { Radian(angle.x),Radian(angle.y),Radian(angle.z) }; }

// 3谺｡蜈・・繧ｯ繝医Ν縺ｨ4x4陦悟・縺ｮ謗帙￠邂・
Vec3 Vec3MulMat4(const Vec3 v, const Mat4& m, const bool isMulW = false);

Mat4 ConvertScalingMat(Vec3 scale);		// 繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ陦悟・繧定ｿ斐☆髢｢謨ｰ
Mat4 ConvertRotationXAxisMat(float angle);	// 蝗櫁ｻ｢陦悟・・倩ｻｸ繧定ｿ斐☆髢｢謨ｰ
Mat4 ConvertRotationYAxisMat(float angle);	// 蝗櫁ｻ｢陦悟・・呵ｻｸ繧定ｿ斐☆髢｢謨ｰ
Mat4 ConvertRotationZAxisMat(float angle);	// 蝗櫁ｻ｢陦悟・・夊ｻｸ繧定ｿ斐☆髢｢謨ｰ
Mat4 ConvertTranslationMat(const Vec3 pos);	// 蟷ｳ陦檎ｧｻ蜍輔ｒ霑斐☆髢｢謨ｰ

Mat4 ConvertRotationMat(const Quaternion q); // 繧ｯ繧ｩ繝ｼ繧ｿ繝九が繝ｳ縺ｧ蝗櫁ｻ｢陦悟・繧堤函謌舌☆繧矩未謨ｰ

// 繝薙Ν繝懊・繝芽｡悟・螟画鋤
Mat4 ConvertBillBoardXAxis();
Mat4 ConvertBillBoardYAxis();
Mat4 ConvertBillBoardZAxis();
Mat4 ConvertBillBoardAllAxis();

Mat4 ConvertViewProjectionMatLookAt(const Vec3 pos, const Vec3 target, const Vec3 up);		// 繝薙Η繝ｼ螟画鋤
Mat4 ConvertViewProjectionMatLookTo(const Vec3 pos, const Vec3 zAxis, const Vec3 yAxis);		// 繝薙Η繝ｼ螟画鋤
Mat4 ConvertPerspectiveProjectionMat(float fovAngle, float aspect, float nearZ, float farZ);	// 騾剰ｦ門ｰ・ｽｱ螟画鋤
Mat4 ConvertOrthographicProjectionMat(float WIN_WIDTH, float WIN_HEIGHT);						// 蟷ｳ陦悟ｰ・ｽｱ螟画鋤
Mat4 ConvertOrthographicProjectionMat(const RectAngle rect, const float nearZ, const float farZ);		// 蟷ｳ陦悟ｰ・ｽｱ螟画鋤
Mat4 ConvertViewportMat(Viewport& viewport);

Mat4 CalculateWorldMat(const Vec3 pos, const Vec3 scale, const Vec3 rot);
Mat4 CalculateWorldMat(const Vec3 pos, const Vec3 scale, const Quaternion rot);

Vec3 operator+(const float num, const Vec3 v);
Vec3 operator-(const float num, const Vec3 v);
Vec3 operator*(const float num, const Vec3 v);
Vec3 operator/(const float num, const Vec3 v);

