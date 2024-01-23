// 3D変換行列
cbuffer ConstBufferDataTransform : register(b0)
{
    matrix mat;
}

// 色
cbuffer ConstBufferDataMaterial : register(b1)
{
    float4 color;
};

struct Appdata
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};