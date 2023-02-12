#pragma once
#include "Vec3.h"

class Vec3;

class Quaternion
{
public:
	float x, y, z, w;

	// �R���X�g���N�^
	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(const float& x, const float& y, const float& z) : x(x), y(y), z(z), w(0) {}
	Quaternion(const float& x, const float& y, const float& z, const float& w) : x(x), y(y), z(z), w(w) {}

	// �֐�
	float Length() const;			// �N�I�[�^�j�I���̑傫��
	Quaternion Norm() const;		// ���K���N�I�[�^�j�I��
	Quaternion Conjugate() const;	// �����N�I�[�^�j�I��
	Quaternion Inverse() const;		// �t�N�I�[�^�j�I��

	// �C�ӎ���]
	Quaternion AnyAxisRotation(const Vec3& v, const float& radian);

	static float Dot(const Quaternion& q1, const Quaternion& q2);		// ��̃N�I�[�^�j�I���̓���
	static Quaternion Identity();	// �P�ʃN�I�[�^�j�I��
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
	static Quaternion MakeAxisAngle(const Vec3& v, const float& radian);
	static Quaternion DirectionToDirection(const Vec3& v1, const Vec3& v2);

	Quaternion operator-() const;

	// �Z�p���Z�q�̃I�[�o�[���[�h
	Quaternion operator+(const Quaternion& other) const;	// ������̃N�I�[�^�j�I���Ƃ̑����Z
	Quaternion operator-(const Quaternion& other) const;	// ������̃N�I�[�^�j�I���Ƃ̈����Z
	Quaternion operator*(const Quaternion& other) const;	// ������̃N�I�[�^�j�I���Ƃ̊|���Z
	Quaternion operator/(const Quaternion& other) const;	// ������̃N�I�[�^�j�I���Ƃ̊���Z
	Quaternion operator*(const float& num) const;			// ��̒l�Ƃ̊|���Z
	Quaternion operator/(const float& num) const;			// ��̒l�Ƃ̊���Z

	// ����������Z�q�̃I�[�o�[���[�h
	Quaternion& operator+=(const Quaternion& other);	// ����������Z +=
	Quaternion& operator-=(const Quaternion& other);	// ����������Z -=
	Quaternion& operator*=(const Quaternion& other);	// ����������Z *=
	Quaternion& operator*=(const float& num);			// ����������Z *=
	Quaternion& operator/=(const float& num);			// ����������Z /=
};

Quaternion operator*(const float& num, const Quaternion& q);