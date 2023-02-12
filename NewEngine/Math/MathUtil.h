#pragma once
#include <math.h>
#include "Vec2.h"
#include "Vec3.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "Viewport.h"

constexpr float PI = 3.141592f;

// ��Βl��Ԃ��֐�
inline float Absolut(const float& num) { return num >= 0 ? num : num * -1; }
inline Vec2 Absolut(const Vec2& num) { return { Absolut(num.x),Absolut(num.y) }; }
inline Vec3 Absolut(const Vec3& num) { return { Absolut(num.x),Absolut(num.y),Absolut(num.z) }; }
// �ʓx�@��x���@�ɕϊ�����֐�
inline float Angle(const float& radian) { return radian * 360 / (2 * PI); }
inline Vec2 Angle(const Vec2& radian) { return { Angle(radian.x),Angle(radian.y) }; }
inline Vec3 Angle(const Vec3& radian) { return { Angle(radian.x),Angle(radian.y),Angle(radian.z) }; }
// �x���@����ʓx�@�ɕϊ�����֐�
inline float Radian(const float& angle) { return angle * (2 * PI) / 360; }
inline Vec2 Radian(const Vec2& angle) { return { Radian(angle.x),Radian(angle.y) }; }
inline Vec3 Radian(const Vec3& angle) { return { Radian(angle.x),Radian(angle.y),Radian(angle.z) }; }

// 3�����x�N�g����4x4�s��̊|���Z
Vec3 Vec3MulMat4(const Vec3& v, const Mat4& m, const bool& isMulW = false);

Mat4 ConvertScalingMat(Vec3 scale);		// �X�P�[�����O�s���Ԃ��֐�
Mat4 ConvertRotationXAxisMat(float angle);	// ��]�s�񂘎���Ԃ��֐�
Mat4 ConvertRotationYAxisMat(float angle);	// ��]�s�񂙎���Ԃ��֐�
Mat4 ConvertRotationZAxisMat(float angle);	// ��]�s�񂚎���Ԃ��֐�
Mat4 ConvertTranslationMat(const Vec3& pos);	// ���s�ړ���Ԃ��֐�

Mat4 ConvertRotationMat(const Quaternion& q); // �N�H�[�^�j�I���ŉ�]�s��𐶐�����֐�

// �r���{�[�h�s��ϊ�
Mat4 ConvertBillBoardXAxis();
Mat4 ConvertBillBoardYAxis();
Mat4 ConvertBillBoardZAxis();
Mat4 ConvertBillBoardAllAxis();

Mat4 ConvertViewProjectionMatLookAt(const Vec3& pos, const Vec3& target, const Vec3& up);		// �r���[�ϊ�
Mat4 ConvertViewProjectionMatLookTo(const Vec3& pos, const Vec3& zAxis, const Vec3& yAxis);		// �r���[�ϊ�
Mat4 ConvertPerspectiveProjectionMat(float fovAngle, float aspect, float nearZ, float farZ);	// �����ˉe�ϊ�
Mat4 ConvertOrthoGrphicProjectionMat(float WIN_WIDTH, float WIN_HEIGHT);						// ���s�ˉe�ϊ�
Mat4 ConvertViewportMat(Viewport& viewport);

Vec3 operator+(const float& num, const Vec3& v);
Vec3 operator-(const float& num, const Vec3& v);
Vec3 operator*(const float& num, const Vec3& v);
Vec3 operator/(const float& num, const Vec3& v);
