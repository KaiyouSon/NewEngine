#include "Mat4.h"
#include "MathUtil.h"

Mat4::Mat4() :
	m{
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0 }
{
}

Mat4::Mat4(
	float _00, float _01, float _02, float _03,
	float _10, float _11, float _12, float _13,
	float _20, float _21, float _22, float _23,
	float _30, float _31, float _32, float _33) :
	m{
		_00,_01,_02,_03,
		_10,_11,_12,_13,
		_20,_21,_22,_23,
		_30,_31,_32,_33 }
{
}

Mat4 Mat4::Zero()
{
	return Mat4();
}

Mat4 Mat4::Identity()
{
	return
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

Mat4 Mat4::Inverse()
{
	float sweepMat[4][8] = {};
	float tmepNum = 0;

	// 掃き出し行列の初期化
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// 引数でもらった行列（左 4 * 4）
			sweepMat[i][j] = m[i][j];

			// 単位行列（右 4 * 4）
			sweepMat[i][j + 4] = (i == j) ? 1.0f : 0.0f;
		}
	}

	// 掃き出し法
	for (int i = 0; i < 4; i++)
	{
		// 最大成分を探索する
		float max = fabsf(sweepMat[i][i]);
		int maxIndex = i;
		for (int j = i + 1; j < 4; j++)
		{
			if (max < fabsf(sweepMat[i][i]))
			{
				max = fabsf(sweepMat[i][i]);
				maxIndex = j;
			}
		}
		// 逆行列求めるかどうか
		if (fabsf(sweepMat[maxIndex][i]) <= 0.000001f)
		{
			// 求めれない場合は単位行列を返す
			return Identity();
		}

		// 対象となる行列の対角成分を1にする
		tmepNum = 1 / sweepMat[i][i];
		for (int j = 0; j < 8; j++)
		{
			sweepMat[i][j] *= tmepNum;
		}

		// 対象となる行列の対角成分以外を0にするため
		for (int j = 0; j < 4; j++)
		{
			if (i == j) continue;

			tmepNum = -sweepMat[j][i];
			for (int k = 0; k < 8; k++)
			{
				sweepMat[j][k] += sweepMat[i][k] * tmepNum;
			}
		}
	}

	// 逆行列を返す
	Mat4 inverseMat = Mat4::Identity();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inverseMat.m[i][j] = sweepMat[i][j + 4];
		}
	}

	return inverseMat;
}

void Mat4::SetXAxis(const Vec3& xAxis) { m[0][0] = xAxis.x; m[0][1] = xAxis.y; m[0][2] = xAxis.z; }
void Mat4::SetYAxis(const Vec3& yAxis) { m[1][0] = yAxis.x;	m[1][1] = yAxis.y; m[1][2] = yAxis.z; }
void Mat4::SetZAxis(const Vec3& zAxis) { m[2][0] = zAxis.x;	m[2][1] = zAxis.y; m[2][2] = zAxis.z; }
void Mat4::SetTranslation(const Vec3& pos) { m[3][0] = pos.x; m[3][1] = pos.y; m[3][2] = pos.z; }

Vec3 Mat4::GetXAxis() const { return { m[0][0],m[0][1] ,m[0][2] }; }
Vec3 Mat4::GetYAxis() const { return { m[1][0],m[1][1] ,m[1][2] }; }
Vec3 Mat4::GetZAxis() const { return { m[2][0],m[2][1] ,m[2][2] }; }
Vec3 Mat4::GetTranslation() const { return { m[3][0],m[3][1] ,m[3][2] }; }

Mat4 Mat4::operator*(const Mat4& other) const
{
	return {
		m[0][0] * other.m[0][0] + m[0][1] * other.m[1][0] + m[0][2] * other.m[2][0] + m[0][3] * other.m[3][0],
		m[0][0] * other.m[0][1] + m[0][1] * other.m[1][1] + m[0][2] * other.m[2][1] + m[0][3] * other.m[3][1],
		m[0][0] * other.m[0][2] + m[0][1] * other.m[1][2] + m[0][2] * other.m[2][2] + m[0][3] * other.m[3][2],
		m[0][0] * other.m[0][3] + m[0][1] * other.m[1][3] + m[0][2] * other.m[2][3] + m[0][3] * other.m[3][3],

		m[1][0] * other.m[0][0] + m[1][1] * other.m[1][0] + m[1][2] * other.m[2][0] + m[1][3] * other.m[3][0],
		m[1][0] * other.m[0][1] + m[1][1] * other.m[1][1] + m[1][2] * other.m[2][1] + m[1][3] * other.m[3][1],
		m[1][0] * other.m[0][2] + m[1][1] * other.m[1][2] + m[1][2] * other.m[2][2] + m[1][3] * other.m[3][2],
		m[1][0] * other.m[0][3] + m[1][1] * other.m[1][3] + m[1][2] * other.m[2][3] + m[1][3] * other.m[3][3],

		m[2][0] * other.m[0][0] + m[2][1] * other.m[1][0] + m[2][2] * other.m[2][0] + m[2][3] * other.m[3][0],
		m[2][0] * other.m[0][1] + m[2][1] * other.m[1][1] + m[2][2] * other.m[2][1] + m[2][3] * other.m[3][1],
		m[2][0] * other.m[0][2] + m[2][1] * other.m[1][2] + m[2][2] * other.m[2][2] + m[2][3] * other.m[3][2],
		m[2][0] * other.m[0][3] + m[2][1] * other.m[1][3] + m[2][2] * other.m[2][3] + m[2][3] * other.m[3][3],

		m[3][0] * other.m[0][0] + m[3][1] * other.m[1][0] + m[3][2] * other.m[2][0] + m[3][3] * other.m[3][0],
		m[3][0] * other.m[0][1] + m[3][1] * other.m[1][1] + m[3][2] * other.m[2][1] + m[3][3] * other.m[3][1],
		m[3][0] * other.m[0][2] + m[3][1] * other.m[1][2] + m[3][2] * other.m[2][2] + m[3][3] * other.m[3][2],
		m[3][0] * other.m[0][3] + m[3][1] * other.m[1][3] + m[3][2] * other.m[2][3] + m[3][3] * other.m[3][3]
	};
}

Mat4 Mat4::operator*(const float& num)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] *= num;
		}
	}

	return *this;
}

Mat4& Mat4::operator*=(const Mat4& other)
{
	*this = *this * other;
	return *this;
}
bool Mat4::operator!=(const Mat4& other) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m[i][j] != other.m[i][j]) return false;
		}
	}
	return true;
}
;
