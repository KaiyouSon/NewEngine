// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewProjMat;
    matrix worldMat;
    float3 cameraPos;
}

// 色
cbuffer ConstantBufferDataColor : register(b1)
{
    float4 color; // 色
}

// UVWパラメーター
cbuffer ConstantBufferDataUVParameter : register(b2)
{
    float2 offset;
    float2 tiling;
};

struct Appdata
{
    float4 pos : POSITION;
    float3 uvw : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float3 uvw : TEXCOORD; // uv値
};