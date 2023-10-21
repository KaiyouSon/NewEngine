// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewMat;
    matrix worldMat;
    float3 cameraPos;
}

// 色
cbuffer ConstantBufferDataColor : register(b1)
{
    float4 color; // 色
}

struct Appdata
{
    float4 pos : POSITION;
};

struct V2P
{
    float4 svpos : SV_POSITION;
};