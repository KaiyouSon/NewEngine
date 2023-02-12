#pragma once
#include "Vec3.h"

class Mat4
{
public:
	float m[4][4] = {};

	Mat4();
	Mat4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33);

	static Mat4 Zero();		// �[���s���Ԃ��֐�
	static Mat4 Identity();	// �P�ʍs���Ԃ��֐�
	Mat4 Inverse();			// �t�s���Ԃ��֐�

	void SetXAxis(const Vec3& xAxis);
	void SetYAxis(const Vec3& yAxis);
	void SetZAxis(const Vec3& zAxis);
	void SetTranslation(const Vec3& pos);

	Vec3 GetXAxis() const;
	Vec3 GetYAxis() const;
	Vec3 GetZAxis() const;
	Vec3 GetTranslation() const;

	// �P�����Z�q�I�[�o�[���[�h
	Mat4 operator*(const Mat4& other) const;
	Mat4 operator*(const float& num);

	// ������Z�q�I�[�o�[���[�h
	Mat4& operator*=(const Mat4& other);

	// ��r���Z�q�̃I�[�o�[���[�h
	bool operator!=(const Mat4& other) const;

};