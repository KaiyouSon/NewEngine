#include "Mat4.h"
#include "MathUtil.h"

Mat4::Mat4() :
	m_{
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
	m_
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

	// �|���o���s��̏�����
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// �����ł�������s��i�� 4 * 4�j
			sweepMat[i][j] = m_[i][j];

			// �P�ʍs��i�E 4 * 4�j
			sweepMat[i][j + 4] = (i == j) ? 1.0f : 0.0f;
		}
	}

	// �|���o���@
	for (int i = 0; i < 4; i++)
	{
		// �ő听����T������
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
		// �t�s�񋁂߂邩�ǂ���
		if (fabsf(sweepMat[maxIndex][i]) <= 1.e-50)
		{
			// ���߂�Ȃ��ꍇ�͒P�ʍs���Ԃ�
			return Identity();
		}

		// �ΏۂƂȂ�s��̑Ίp������1�ɂ���
		tmepNum = 1 / sweepMat[i][i];
		for (int j = 0; j < 8; j++)
		{
			sweepMat[i][j] *= tmepNum;
		}

		// �ΏۂƂȂ�s��̑Ίp�����ȊO��0�ɂ��邽��
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

	// �t�s���Ԃ�
	Mat4 inverseMat = Mat4::Identity();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inverseMat.m_[i][j] = sweepMat[i][j + 4];
		}
	}

	return inverseMat;
}

void Mat4::SetXAxis(const Vec3 xAxis) { m_[0][0] = xAxis.x; m_[0][1] = xAxis.y; m_[0][2] = xAxis.z; }
void Mat4::SetYAxis(const Vec3 yAxis) { m_[1][0] = yAxis.x;	m_[1][1] = yAxis.y; m_[1][2] = yAxis.z; }
void Mat4::SetZAxis(const Vec3 zAxis) { m_[2][0] = zAxis.x;	m_[2][1] = zAxis.y; m_[2][2] = zAxis.z; }
void Mat4::SetTranslation(const Vec3 pos) { m_[3][0] = pos.x; m_[3][1] = pos.y; m_[3][2] = pos.z; }

Vec3 Mat4::GetXAxis() const { return { m_[0][0],m_[0][1] ,m_[0][2] }; }
Vec3 Mat4::GetYAxis() const { return { m_[1][0],m_[1][1] ,m_[1][2] }; }
Vec3 Mat4::GetZAxis() const { return { m_[2][0],m_[2][1] ,m_[2][2] }; }
Vec3 Mat4::GetTrans() const { return { m_[3][0],m_[3][1] ,m_[3][2] }; }
Vec3 Mat4::GetScale() const { return { m_[0][0],m_[1][1] ,m_[2][2] }; }

Mat4 Mat4::operator*(const Mat4& other) const
{
	return {
		m_[0][0] * other.m_[0][0] + m_[0][1] * other.m_[1][0] + m_[0][2] * other.m_[2][0] + m_[0][3] * other.m_[3][0],
		m_[0][0] * other.m_[0][1] + m_[0][1] * other.m_[1][1] + m_[0][2] * other.m_[2][1] + m_[0][3] * other.m_[3][1],
		m_[0][0] * other.m_[0][2] + m_[0][1] * other.m_[1][2] + m_[0][2] * other.m_[2][2] + m_[0][3] * other.m_[3][2],
		m_[0][0] * other.m_[0][3] + m_[0][1] * other.m_[1][3] + m_[0][2] * other.m_[2][3] + m_[0][3] * other.m_[3][3],

		m_[1][0] * other.m_[0][0] + m_[1][1] * other.m_[1][0] + m_[1][2] * other.m_[2][0] + m_[1][3] * other.m_[3][0],
		m_[1][0] * other.m_[0][1] + m_[1][1] * other.m_[1][1] + m_[1][2] * other.m_[2][1] + m_[1][3] * other.m_[3][1],
		m_[1][0] * other.m_[0][2] + m_[1][1] * other.m_[1][2] + m_[1][2] * other.m_[2][2] + m_[1][3] * other.m_[3][2],
		m_[1][0] * other.m_[0][3] + m_[1][1] * other.m_[1][3] + m_[1][2] * other.m_[2][3] + m_[1][3] * other.m_[3][3],

		m_[2][0] * other.m_[0][0] + m_[2][1] * other.m_[1][0] + m_[2][2] * other.m_[2][0] + m_[2][3] * other.m_[3][0],
		m_[2][0] * other.m_[0][1] + m_[2][1] * other.m_[1][1] + m_[2][2] * other.m_[2][1] + m_[2][3] * other.m_[3][1],
		m_[2][0] * other.m_[0][2] + m_[2][1] * other.m_[1][2] + m_[2][2] * other.m_[2][2] + m_[2][3] * other.m_[3][2],
		m_[2][0] * other.m_[0][3] + m_[2][1] * other.m_[1][3] + m_[2][2] * other.m_[2][3] + m_[2][3] * other.m_[3][3],

		m_[3][0] * other.m_[0][0] + m_[3][1] * other.m_[1][0] + m_[3][2] * other.m_[2][0] + m_[3][3] * other.m_[3][0],
		m_[3][0] * other.m_[0][1] + m_[3][1] * other.m_[1][1] + m_[3][2] * other.m_[2][1] + m_[3][3] * other.m_[3][1],
		m_[3][0] * other.m_[0][2] + m_[3][1] * other.m_[1][2] + m_[3][2] * other.m_[2][2] + m_[3][3] * other.m_[3][2],
		m_[3][0] * other.m_[0][3] + m_[3][1] * other.m_[1][3] + m_[3][2] * other.m_[2][3] + m_[3][3] * other.m_[3][3]
	};
}

Mat4 Mat4::operator*(const float num)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_[i][j] *= num;
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
			if (m_[i][j] != other.m_[i][j]) return false;
		}
	}
	return true;
}
;
