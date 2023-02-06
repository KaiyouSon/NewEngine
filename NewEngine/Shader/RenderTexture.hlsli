
// 3D変換行列
cbuffer ConstBufferDataTransform : register(b0)
{
	matrix mat; // 3D変換行列
}

// マテリアル
cbuffer ConstBufferDataMaterial : register(b1)
{
    float4 color; // 色(RGBA)
};

// 頂点シェーダーからの出力構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};