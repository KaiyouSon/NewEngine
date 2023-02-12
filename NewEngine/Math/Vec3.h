#pragma once
#include "Vec2.h"
#include "Quaternion.h"

class Vec2;
class Quaternion;

class Vec3
{
public:
	// �ϐ�
	float x; // �x�N�g���� x ����
	float y; // �x�N�g���� y ����
	float z; // �x�N�g���� z ����

	// �R���X�g���N�^
	constexpr Vec3() : x(0), y(0), z(0) {};
	constexpr Vec3(const float& num) : x(num), y(num), z(num) {};
	constexpr Vec3(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {};

	// static�ϐ�
	const static Vec3 left;    // Vec3(-1,0,0) �Ɠ����Ӗ�
	const static Vec3 right;   // Vec3(1,0,0) �Ɠ����Ӗ�
	const static Vec3 up;      // Vec3(0,1,0) �Ɠ����Ӗ�
	const static Vec3 down;    // Vec3(0,-1,0) �Ɠ����Ӗ�
	const static Vec3 front; // Vec3(0,0,1) �Ɠ����Ӗ�
	const static Vec3 back;    // Vec3(0,0,-1)�Ɠ����Ӗ�
	const static Vec3 one;     // Vec3(1,1,1) �Ɠ����Ӗ�
	const static Vec3 zero;    // Vec3(0,0,0) �Ɠ����Ӗ�

	// �֐�
	float Length() const;    // �x�N�g���̑傫��
	Vec3 Norm() const;    // ���K���ɂ����x�N�g��

	// static�֐�
	static float Dot(const Vec3& v1, const Vec3& v2);		// ��̃x�N�g���̓���
	static Vec3 Cross(const Vec3& v1, const Vec3& v2);		// ��̃x�N�g���̊O��
	static float Distance(const Vec3& v1, const Vec3& v2);	// ��̃x�N�g���̋���

	// ��̃x�N�g���Ŋe�����̈�ԑ傫�Ȓl���g�p���ăx�N�g�����쐬����
	static Vec3 Max(const Vec3& v1, const Vec3& v2);
	// ��̃x�N�g���Ŋe�����̈�ԏ����Ȓl���g�p���ăx�N�g�����쐬����
	static Vec3 Min(const Vec3& v1, const Vec3& v2);

	// �Z�p���Z�q�̃I�[�o�[���[�h
	Vec3 operator+(const Vec3& other) const; // ��������̃x�N�g���Ƃ̑����Z
	Vec3 operator-(const Vec3& other) const; // ��������̃x�N�g���Ƃ̈����Z
	Vec3 operator*(const Vec3& other) const; // ��������̃x�N�g���Ƃ̊|���Z
	Vec3 operator/(const Vec3& other) const; // ��������̃x�N�g���Ƃ̊���Z
	Vec3 operator+(const Vec2& other) const; // ��������̃x�N�g���Ƃ̑����Z
	Vec3 operator-(const Vec2& other) const; // ��������̃x�N�g���Ƃ̈����Z
	Vec3 operator*(const Vec2& other) const; // ��̒l�Ƃ̊|���Z
	Vec3 operator/(const Vec2& other) const; // ��̒l�Ƃ̊���Z
	Vec3 operator+(float num) const;		 // ��̒l�Ƃ̑����Z
	Vec3 operator-(float num) const;		 // ��̒l�Ƃ̈����Z
	Vec3 operator*(float num) const;         // ��̒l�Ƃ̊|���Z
	Vec3 operator/(float num) const;         // ��̒l�Ƃ̊���Z
	Vec3 operator-() const;

	// ����������Z�q�̃I�[�o�[���[�h
	Vec3& operator+=(const Vec3& other); // ����������Z +=
	Vec3& operator-=(const Vec3& other); // ����������Z -=
	Vec3& operator+=(float num);         // ����������Z +=
	Vec3& operator-=(float num);         // ����������Z -=
	Vec3& operator*=(float num);         // ����������Z *=
	Vec3& operator/=(float num);         // ����������Z /=

	// ������Z�q�̃I�[�o�[���[�h
	Vec3& operator=(float num);			// ��̒l����
	Vec3& operator=(const Vec2& other); // Vec2��Vec3�ɕϊ�
	Vec3& operator=(const Quaternion& q); // Quaternion��Vec3�ɕϊ�

	// �C���N�������g/�f�N�������g���Z�q�̃I�[�o�[���[�h
	Vec3& operator++();   // �O�u�C���N�������g
	Vec3 operator++(int); // ��u�C���N�������g
	Vec3& operator--();   // �O�u�f�N�������g
	Vec3 operator--(int); // ��u�f�N�������g

	// ��r���Z�q�̃I�[�o�[���[�h
	bool operator==(const Vec3& other);
	bool operator!=(const Vec3& other);
	bool operator>=(const Vec3& other);
	bool operator<=(const Vec3& other);
	bool operator==(float num);
	bool operator!=(float num);
	bool operator>=(float num);
	bool operator<=(float num);
};
