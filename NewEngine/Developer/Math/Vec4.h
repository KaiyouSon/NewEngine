#pragma once
class Vec4
{
public:
	// �ϐ�
	float x; // �x�N�g���� x ����
	float y; // �x�N�g���� y ����
	float z; // �x�N�g���� z ����
	float w; // �x�N�g���� w ����

	// �R���X�g���N�^
	Vec4() : x(0), y(0), z(0), w(0) {};
	Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

	// �֐�
	float Magnitude() const;    // �x�N�g���̑傫��
	float SqrMagnitude() const; // �x�N�g���̑傫���̓��
	Vec4 Normalized() const;    // ���K���ɂ����x�N�g��
	Vec4 Inverse();

	// static�֐�
	static float Dot(const Vec4& v1, const Vec4& v2);      // ��̃x�N�g���̓���
	static float Distance(const Vec4& v1, const Vec4& v2); // ��̃x�N�g���̋���

	// ��̃x�N�g���Ŋe�����̈�ԑ傫�Ȓl���g�p���ăx�N�g�����쐬����
	static Vec4 Max(const Vec4& v1, const Vec4& v2);
	// ��̃x�N�g���Ŋe�����̈�ԏ����Ȓl���g�p���ăx�N�g�����쐬����
	static Vec4 Min(const Vec4& v1, const Vec4& v2);


	// �Z�p���Z�q�̃I�[�o�[���[�h
	Vec4 operator+(const Vec4& other) const; // ��������̃x�N�g���Ƃ̑����Z
	Vec4 operator-(const Vec4& other) const; // ��������̃x�N�g���Ƃ̈����Z
	Vec4 operator*(const Vec4& other) const; // ��������̃x�N�g���Ƃ̊|���Z
	Vec4 operator/(const Vec4& other) const; // ��������̃x�N�g���Ƃ̊���Z
	Vec4 operator*(float num) const;         // ��̒l�Ƃ̊|���Z
	Vec4 operator/(float num) const;         // ��̒l�Ƃ̊���Z

	// ����������Z�q�̃I�[�o�[���[�h
	Vec4& operator+=(const Vec4& other); // ����������Z +=
	Vec4& operator-=(const Vec4& other); // ����������Z -=
	Vec4& operator+=(float num);         // ����������Z +=
	Vec4& operator-=(float num);         // ����������Z -=
	Vec4& operator*=(float num);         // ����������Z *=
	Vec4& operator/=(float num);         // ����������Z /=

	// ������Z�q�̃I�[�o�[���[�h
	Vec4& operator=(float num); // ��̒l����

	// �C���N�������g/�f�N�������g���Z�q�̃I�[�o�[���[�h
	Vec4& operator++();   // �O�u�C���N�������g
	Vec4 operator++(int); // ��u�C���N�������g
	Vec4& operator--();   // �O�u�f�N�������g
	Vec4 operator--(int); // ��u�f�N�������g

	// ��r���Z�q�̃I�[�o�[���[�h
	bool operator==(const Vec4& other);
	bool operator!=(const Vec4& other);
	bool operator>=(const Vec4& other);
	bool operator<=(const Vec4& other);
	bool operator==(float num);
	bool operator!=(float num);
	bool operator>=(float num);
	bool operator<=(float num);
};