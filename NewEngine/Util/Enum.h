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
enum class Flip
{

};
