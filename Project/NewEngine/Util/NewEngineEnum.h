#pragma once

// 繝悶Ξ繝ｳ繝峨Δ繝ｼ繝・
enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
};

// 譎る俣蜊倅ｽ・
enum class TimeUnit
{
	MilliSecond,	// 繝溘Μ遘・
	Second,			// 遘・
	Minute			// 蛻・
};

// 繝吶ず繧ｨ譖ｲ邱壹・遞ｮ鬘・
enum class BezierType
{
	// 繧､繝ｼ繧ｸ繝ｳ繧ｰ縺ｪ縺・
	Lerp,

	// 繧､繝ｼ繧ｸ繝ｳ繧ｰ縺ゅｊ
	EaseIn,
	EaseOut,
};

// 繧､繝ｼ繧ｸ繝ｳ繧ｰ遞ｮ鬘・
enum class EaseType
{
	Lerp,
	In,
	Out,
	InOut,
};

// 繝薙Ν繝懊・繝峨・遞ｮ鬘・
enum class BillboardType
{
	None,
	XAxisBillboard,
	YAxisBillboard,
	ZAxisBillboard,
	AllAxisBillboard,
};

// 繝医・繝ｭ繧ｸ繝ｼ繧ｿ繧､繝・
enum class TopologyType
{
	Point,
	LineList,
	LineStrip,
	TriangleList,
	TriangleStrip,
};

// 繧ｫ繝ｼ繝ｪ繝ｳ繧ｰ繝｢繝ｼ繝・
enum class CullMode
{
	None,
	Front,
	Back,
};

// 繝｢繝・Ν縺ｮ繝輔か繝ｼ繝槭ャ繝・
enum class ModelFormat
{
	Obj,
	Fbx,
};

// 蜿崎ｻ｢繝輔Λ繧ｰ
enum class FlipType
{
	None,
	X,
	Y,
	XY
};

// 繧ｳ繝ｩ繧､繝繝ｼ縺ｮ遞ｮ鬘・
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

// 繧ｭ繝･繝ｼ繝悶・蜈ｫ鬆らせ
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

enum class ViewType
{
	SRV,
	RTV,
	DSV,
	UAV
};
