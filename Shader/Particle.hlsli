
// 3D変換行列
cbuffer ConstBufferDataTransform : register(b0)
{
	matrix mat; // 3D変換行列
}

// 頂点シェーダーの出力構造体
// （頂点シェーダーからピクセルシェーダーヘのやり取りに使用する）
struct VSOutput
{
	float4 pos : POSITION;	// システム用頂点座標
	float scale : TEXCOORD;	// スケール
	float4 color : COLOR;
};

// ジオメトリシェーダーへの出力
struct GSOutput
{
	float4 svpos : SV_POSITION;	// システム用頂点座標
	float2 uv : TEXCOORD;		// uv値
	float4 color : COLOR;
};