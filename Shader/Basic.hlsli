
// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
	matrix mat; // 3D変換行列
	//matrix viewMat;
	//matrix worldMat;
	//float3 cameraPos;
}

// マテリアル
cbuffer ConstantBufferDataMaterial : register(b1)
{
	float4 color; // 色(RGBA)
};

//// ライト
//cbuffer ConstBufferDataLight : register(b2)
//{
//	float3 lightVec;	// ライトへの方向の単位ベクトル
//	float3 lightColor;	// ライトの色(RGB)
//};

// 頂点シェーダーの出力構造体
// （頂点シェーダーからピクセルシェーダーヘのやり取りに使用する）
struct VSOutput
{
	float4 svpos : SV_POSITION;	// システム用頂点座標
	float3 normal : NORMAL;		// 法線ベクトル
	float2 uv : TEXCOORD;		// uv値
};