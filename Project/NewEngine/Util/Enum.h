#pragma once

// �u�����h���[�h
enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
};

// �p�C�v���C�������p
enum class PipelineBlend
{
	Alpha = 1,
	Add = 2,
	Sub = 4,
	Inv = 8,
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

// �C�[�W���O���
enum class EaseType
{
	Lerp,
	In,
	Out,
	InOut,
};

// �r���{�[�h�̎��
enum class BillboardType
{
	None,
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

// ���f���̃t�H�[�}�b�g
enum class ModelFormat
{
	Obj,
	Fbx,
	DFbx,
};

// ���]�t���O
enum class FlipType
{
	None,
	X,
	Y,
	XY
};

// �R���C�_�[�̎��
enum class ColliderPrimitive
{
	// 2D
	Circle,
	Square,

	// 3D
	Sphere,
	Cube,
	Ray,
	Plane,
	Triangle,
	Capsule,
};

// �L���[�u�̔����_
enum class CubePoints
{
	FLT, FLD,
	FRT, FRD,
	BLT, BLD,
	BRT, BRD
};

enum class MouseCode
{
	Left = 0,
	Right = 1,
	Wheel = 2,
};