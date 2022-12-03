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

Mat4 Mat4::Inverse(const Mat4& mat)
{
	float sweepMat[4][8] = {};
	float tmpNum = 0;

	// �|���o���s��̏�����
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// �����ł�������s��i�� 4 * 4�j
			sweepMat[i][j] = mat.m[i][j];

			// �P�ʍs��i�E 4 * 4�j
			sweepMat[i][j + 4] = (i == j) ? 1.0f : 0.0f;
		}
	}

	// �|���o���@
	for (int i = 0; i < 4; i++)
	{
		// �ő听����T������
		float max = mathUtil->Absolut(sweepMat[i][i]);
		int maxIndex = i;
		for (int j = i + 1; j < 4; j++)
		{
			if (max < mathUtil->Absolut(sweepMat[i][i]))
			{
				max = mathUtil->Absolut(sweepMat[i][i]);
				maxIndex = j;
			}
		}
		// �t�s�񋁂߂邩�ǂ���
		if (mathUtil->Absolut(sweepMat[maxIndex][i]) <= 0.000001f)
		{
			// ���߂�Ȃ��ꍇ�͒P�ʍs���Ԃ�
			return Identity();
		}

		// �ΏۂƂȂ�s��̑Ίp������1�ɂ���
		tmpNum = 1 / sweepMat[i][i];
		for (int j = 0; j < 8; j++)
		{
			sweepMat[i][j] *= tmpNum;
		}

		// �ΏۂƂȂ�s��̑Ίp�����ȊO��0�ɂ��邽��
		for (int j = 0; j < 4; j++)
		{
			if (i == j) continue;

			tmpNum = -sweepMat[j][i];
			for (int k = 0; k < 8; k++)
			{
				sweepMat[j][k] += sweepMat[i][k] * tmpNum;
			}
		}
	}

	// �t�s���Ԃ�
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
