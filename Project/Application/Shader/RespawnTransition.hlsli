// 3D変換行列
cbuffer ConstBufferDataTransform : register(b0)
{
    matrix mat; // 3D変換行列
}

// 色
cbuffer ConstBufferDataMaterial : register(b1)
{
    float4 color; // 色(RGBA)
};

// 色
cbuffer ConstBufferDataTransition : register(b2)
{
    float min;
    float max;
};

// 色
cbuffer ConstBufferDataUV : register(b3)
{
    float2 offset;
    float2 tiling;
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