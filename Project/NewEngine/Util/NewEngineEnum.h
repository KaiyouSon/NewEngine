#pragma once

// オブジェクトタイプ
enum class GameObjectType
{
	Object3D,
	Sprite,
	ParticleMesh,
	Camera,
};

// コンポネントの種類
enum class ComponentType
{
	Unkwone,

	// オブジェクト情報
	Object3DInfo,
	SpriteInfo,
	ParticleMeshInfo,
	CameraInfo,

	Transform,
	TextureData,
	ModelData,
	PipelineData,
	ParticleData,
};

enum class CameraType
{
	Perspective,	// 3D用
	Orthographic,	// 2D用
};

// ブレンドモード
enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
	Count,
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

// 塗りつぶし
enum class FillMode
{
	Solid,
	Wireframe,
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
	Material,
	Depth,
	Render,
	Volume,
	Count,
};

enum class ShaderType
{
	Vertex,
	Geometry,
	Pixel,
	Compute,
	Count,
};

enum class LightType
{
	DirectionalLight,
	PointLight,
	SpotLight
};

enum class LayerType
{
	Pass,	// パスを描画する
	Scene,	// シーンを描画する
};

enum class FrustamPlanePos
{
	Left, Right,
	Top, Down,
	Near, Far
};

enum class GraphicsType
{
	DistanceFog,
};

enum class PipelineType
{
	Graphics,
	Compute
};