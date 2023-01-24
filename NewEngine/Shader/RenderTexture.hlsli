
cbuffer cbuff0 : register(b0)
{
};

cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat; // 3D変換行列
}

// 頂点シェーダーからの出力構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};