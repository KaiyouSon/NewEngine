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
	m
	{
		_00,_01,_02,_03,
		_10,_11,_12,_13,
		_20,_21,_22,_23,
		_30,_31,_32,_33
	}
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

	// 謗・″蜃ｺ縺苓｡悟・縺ｮ蛻晄悄蛹・
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// 蠑墓焚縺ｧ繧ゅｉ縺｣縺溯｡悟・・亥ｷｦ 4 * 4・・
			sweepMat[i][j] = m[i][j];

			// 蜊倅ｽ崎｡悟・・亥承 4 * 4・・
			sweepMat[i][j + 4] = (i == j) ? 1.0f : 0.0f;
		}
	}

	// 謗・″蜃ｺ縺玲ｳ・
	for (int i = 0; i < 4; i++)
	{
		// 譛螟ｧ謌仙・繧呈爾邏｢縺吶ｋ
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
		// 騾・｡悟・豎ゅａ繧九°縺ｩ縺・°
		if (fabsf(sweepMat[maxIndex][i]) <= 1.e-50)
		{
			// 豎ゅａ繧後↑縺・ｴ蜷医・蜊倅ｽ崎｡悟・繧定ｿ斐☆
			return Identity();
		}

		// 蟇ｾ雎｡縺ｨ縺ｪ繧玖｡悟・縺ｮ蟇ｾ隗呈・蛻・ｒ1縺ｫ縺吶ｋ
		tmepNum = 1 / sweepMat[i][i];
		for (int j = 0; j < 8; j++)
		{
			sweepMat[i][j] *= tmepNum;
		}

		// 蟇ｾ雎｡縺ｨ縺ｪ繧玖｡悟・縺ｮ蟇ｾ隗呈・蛻・ｻ･螟悶ｒ0縺ｫ縺吶ｋ縺溘ａ
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

	// 騾・｡悟・繧定ｿ斐☆
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

Mat4 Mat4::Transpose()
{
	Mat4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = m[j][i];
		}
	}
	return result;
}

void Mat4::SetXAxis(const Vec3 xAxis) { m[0][0] = xAxis.x; m[0][1] = xAxis.y; m[0][2] = xAxis.z; }
void Mat4::SetYAxis(const Vec3 yAxis) { m[1][0] = yAxis.x;	m[1][1] = yAxis.y; m[1][2] = yAxis.z; }
void Mat4::SetZAxis(const Vec3 zAxis) { m[2][0] = zAxis.x;	m[2][1] = zAxis.y; m[2][2] = zAxis.z; }
void Mat4::SetTranslation(const Vec3 pos) { m[3][0] = pos.x; m[3][1] = pos.y; m[3][2] = pos.z; }

Vec3 Mat4::GetXAxis() const { return { m[0][0],m[0][1] ,m[0][2] }; }
Vec3 Mat4::GetYAxis() const { return { m[1][0],m[1][1] ,m[1][2] }; }
Vec3 Mat4::GetZAxis() const { return { m[2][0],m[2][1] ,m[2][2] }; }
Vec3 Mat4::GetTrans() const { return { m[3][0],m[3][1] ,m[3][2] }; }
Vec3 Mat4::GetScale() const { return { m[0][0],m[1][1] ,m[2][2] }; }

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

Mat4 Mat4::operator*(const float num)
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

