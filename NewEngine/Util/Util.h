#pragma once
#include "Color.h"
#include "FrameRate.h"
#include "Random.h"
#include "Singleton.h"
#include "MathUtil.h"
#include "Transform.h"
#include "Camera.h"
#include "SceneManager.h"
#include <memory>
#include <vector>
#include <string>
#include <list>

enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
	Screen
};

// ��r���đ傫������Ԃ�
float Max(const float& a, const float& b);

// ��r���ď���������Ԃ�
float Min(const float& a, const float& b);

// ������Ԃ��i -1, 0, 1 �j
int Sign(const float& a);

// �l��Min��Max�̊Ԃɐ�������֐�
float Clamp(const float& value, const float& min = 0, const float& max = 1);

// �l����������֐�
float Convergence(const float& value, const float& speed, const float& origin = 0);

// �������擾
int GetDight(const int& value);

// ���[���h���W���X�N���[�����W�ɕϊ�����
Vec2 WorldToScreen(const Vec3& worldPos);