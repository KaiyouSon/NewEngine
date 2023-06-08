#pragma once

// �u�����h���[�h
enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
};

// ���ԒP��
enum class TimeUnit
{
	MilliSecond,	// �~���b
	Second,			// �b
	Minute			// ��
};

// �x�W�G�Ȑ��̎��
enum class BezierType
{
	// �C�[�W���O�Ȃ�
	Lerp,

	// �C�[�W���O����
	EaseIn,
	EaseOut,
};

// �r���{�[�h�̎��
enum class BillBoardType
{
	XAxisBillboard,
	YAxisBillboard,
	ZAxisBillboard,
	AllAxisBillboard,
};

// �g�|���W�[�^�C�v
enum class TopologyType
{
	Point,
	Line,
	Triangle,
};

// �J�[�����O���[�h
enum class CullMode
{
	None,
	Front,
	Back,
};
