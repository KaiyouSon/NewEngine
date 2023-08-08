#pragma once

// ブレンドモード
enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
};

// パイプライン生成用
enum class PipelineBlend
{
	Alpha = 1,
	Add = 2,
	Sub = 4,
	Inv = 8,
};

// 時間単位
enum class TimeUnit
{
	MilliSecond,	// ミリ秒
	Second,			// 秒
	Minute			// 分
};

// ベジエ曲線の種類
enum class BezierType
{
	// イージングなし
	Lerp,

	// イージングあり
	EaseIn,
	EaseOut,
};

// イージング種類
enum class EaseType
{
	Lerp,
	In,
	Out,
	InOut,
};

// ビルボードの種類
enum class BillboardType
{
	None,
	XAxisBillboard,
	YAxisBillboard,
	ZAxisBillboard,
	AllAxisBillboard,
};

// トポロジータイプ
enum class TopologyType
{
	Point,
	Line,
	Triangle,
};

// カーリングモード
enum class CullMode
{
	None,
	Front,
	Back,
};

// モデルのフォーマット
enum class ModelFormat
{
	Obj,
	Fbx,
	DFbx,
};

// 反転フラグ
enum class FlipType
{
	None,
	X,
	Y,
	XY
};

// コライダーの種類
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

// キューブの八頂点
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