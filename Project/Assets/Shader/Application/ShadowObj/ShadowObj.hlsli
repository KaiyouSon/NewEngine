// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewProjMat;
    matrix worldMat;
}

cbuffer ConstantBufferDataShadowObj : register(b1)
{
    matrix lightViewProjMat;
    float3 cameraPos;
}

struct Appdata
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 wpos : POSITION0; // ワールド座標
    float3 normal : NORMAL; // 法線ベクトル
    float2 uv : TEXCOORD0; // uv値
    float2 depth : TEXCOORD1;
};
