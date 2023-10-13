// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewMat;
    matrix worldMat;
    matrix billboardMat;
}

// 色
cbuffer ConstantBufferDataColor : register(b1)
{
    float4 color; // 色
}

struct Appdata
{
    float4 pos : POSITION;
    float2 scale : TEXCOORD0;
    float2 timer : TEXCOORD1;
};

struct V2G
{
    float4 pos : SV_POSITION; // システム用頂点座標
    float2 scale : TEXCOORD0; // スケール
    float2 timer : TEXCOORD1;
};

struct G2P
{
    float4 pos : SV_POSITION; //システム用頂点座標
    float2 uv : TEXCOORD; //uv値
};
