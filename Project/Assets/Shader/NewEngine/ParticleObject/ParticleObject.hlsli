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

// UV情報
cbuffer ConstantBufferDataUVParameter : register(b2)
{
    float2 offset;
    float2 tiling;
};

struct V2G
{
    float4 pos : SV_POSITION; // システム用頂点座標
    float2 scale : TEXCOORD0; // スケール
    float shininess : TEXCOORD1; // 輝度
    float4 color : COLOR; //色
};

struct G2P
{
    float4 pos : SV_POSITION; //システム用頂点座標
    float2 uv : TEXCOORD0; //uv値
    float shininess : TEXCOORD1; // 輝度
    float4 color : COLOR; //色
};

