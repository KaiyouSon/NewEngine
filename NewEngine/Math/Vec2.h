#pragma once
#include "Vec3.h"

class Vec3;

class Vec2
{
public:
	// �ϐ�
	float x; // �x�N�g���� x ����
	float y; // �x�N�g���� y ����

	// static�ϐ�
	const static Vec2 left;		// Vector2(-1, 0) �Ɠ����Ӗ�
	const static Vec2 right;	// Vector2(1, 0) �Ɠ����Ӗ�
	const static Vec2 up;		// Vector2(0, 1) �Ɠ����Ӗ�
	const static Vec2 down;		// Vector2(0, -1) �Ɠ����Ӗ�
	const static Vec2 one;		// Vector2(1, 1) �Ɠ����Ӗ�
	const static Vec2 zero;		// Vector2(0, 0) �Ɠ����Ӗ�

	// �R���X�g���N�^
	constexpr Vec2() : x(0), y(0) {};
	constexpr Vec2(const float& num) : x(num), y(num) {};
	constexpr Vec2(const float& x, const float& y) : x(x), y(y) {};

	// �֐�
	float Length() const;	// �x�N�g���̑傫��
	Vec2 Norm() const;	// ���K���ɂ����x�N�g��

	// static�֐�
	static float Dot(const Vec2& v1, const Vec2& v2);		// ��̃x�N�g���̓���
	static float Cross(const Vec2& v1, const Vec2& v2);		// ��̃x�N�g���̊O��
	static float Distance(const Vec2& v1, const Vec2& v2);	// ��̃x�N�g���̋���

	// ��̃x�N�g���Ŋe�����̈�ԑ傫�Ȓl���g�p���ăx�N�g�����쐬����
	static Vec2 Max(const Vec2& v1, const Vec2& v2);
	// ��̃x�N�g���Ŋe�����̈�ԏ����Ȓl���g�p���ăx�N�g�����쐬����
	static Vec2 Min(const Vec2& v1, const Vec2& v2);

	// �Z�p���Z�q�̃I�[�o�[���[�h
	Vec2 operator+(const Vec2& other) const; // ��������̃x�N�g���Ƃ̑����Z
	Vec2 operator-(const Vec2& other) const; // ��������̃x�N�g���Ƃ̈����Z
	Vec2 operator*(const Vec2& other) const; // ��������̃x�N�g���Ƃ̊|���Z
	Vec2 operator/(const Vec2& other) const; // ��������̃x�N�g���Ƃ̊���Z
	Vec2 operator*(float num) const;	// ��̒l�Ƃ̊|���Z
	Vec2 operator/(float num) const;	// ��̒l�Ƃ̊���Z

	// ����������Z�q�̃I�[�o�[���[�h
	Vec2& operator +=(const Vec2& other); // ����������Z +=
	Vec2& operator -=(const Vec2& other); // ����������Z -=
	Vec2& operator +=(float num); // ����������Z +=
	Vec2& operator -=(float num); // ����������Z -=
	Vec2& operator *=(float num); // ����������Z *=
	Vec2& operator /=(float num); // ����������Z /=

	// ������Z�q�̃I�[�o�[���[�h
	Vec2& operator=(float num);		  // ��̒l����
	Vec2& operator=(const Vec3& vec); // ��̒l����

	// �C���N�������g/�f�N�������g���Z�q�̃I�[�o�[���[�h
	Vec2& operator++(); // �O�u�C���N�������g
	Vec2 operator++(int); // ��u�C���N�������g
	Vec2& operator--(); // �O�u�f�N�������g
	Vec2 operator--(int); // ��u�f�N�������g

	// ��r���Z�q�̃I�[�o�[���[�h
	bool operator ==(const Vec2& other);
	bool operator !=(const Vec2& other);
	bool operator >=(const Vec2& other);
	bool operator <=(const Vec2& other);
	bool operator ==(float num);
	bool operator !=(float num);
	bool operator >=(float num);
	bool operator <=(float num);

};