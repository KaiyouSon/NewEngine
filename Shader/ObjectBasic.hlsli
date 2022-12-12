
cbuffer ConstBufferDataTransform : register(b0)
{
	matrix mat;// 3D変換行列
}

cbuffer ConstBufferDataB1 : register(b1)
{
	float3 ambient : packoffset(c0);	// アンビエント係数
	float3 diffuse : packoffset(c1);	// ディフューズ係数
	float3 specular : packoffset(c2);	// スペキュラー係数
	float alpha : packoffset(c2.w);	// アルファ
}
// マテリアル
//cbuffer ConstBufferDataMaterial : register(b0)
//{
//	float4 color; // 色(RGBA)
//};
//
//// 3D変換行列
//cbuffer ConstBufferDataTransform : register(b1)
//{
//	matrix mat; // 3D変換行列
//}

// 頂点シェーダーの出力構造体
// （頂点シェーダーからピクセルシェーダーヘのやり取りに使用する）
struct VSOutput
{
	float4 svpos : SV_POSITION;	// システム用頂点座標
	//float3 normal : NORMAL;		// 法線ベクトル
	float4 color: COLOR;
	float2 uv : TEXCOORD;		// uv値
};