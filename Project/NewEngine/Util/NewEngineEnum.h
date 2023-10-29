#pragma once

// ブレンドモード
enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
};

// 時間単位
enum class TimeUnit
{
	MilliSecond, // ミリ秒
	Second,      // 秒
	Minute       // 分
};

// ベジェ曲線の種類
enum class BezierType
{
	// 線形補間
	Lerp,

	// イーズイン
	EaseIn,
	// イーズアウト
	EaseOut,
};

// イーズの種類
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

// トポロジの種類
enum class TopologyType
{
	Point,
	LineList,
	LineStrip,
	TriangleList,
	TriangleStrip,
};

// カリングの種類
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
};

// フリップの種類
enum class FlipType
{
	None,
	X,
	Y,
	XY
};

// コライダーのプリミティブ
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

// キューブの頂点
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

enum class TextureType
{
	Default,
	Color,
	Material,
	Depth,
	Render,
	Volume,
};

enum class ShaderType
{
	Compute,
	Vertex,
	Geometry,
	Pixel,
	Size,
};

enum class LightType
{
	DirectionalLight,
	PointLight,
	SpotLight
};