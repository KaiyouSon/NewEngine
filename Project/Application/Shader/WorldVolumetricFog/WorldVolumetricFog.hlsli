// 平行光源
struct DirectionalLight
{
    float4 color;
    float3 vec;
    uint isActive;
};

// 点光源
struct PointLight
{
    float4 color;
    float3 pos;
    float radius;
    float3 colorRate;
    float decay;
    uint isActive;
};

// スポットライト
struct SpotLight
{
    float4 color;
    float3 vec;
    float radius;
    float3 pos;
    float decay;
    float3 colorRate;
    uint isActive;
    float2 cosAngle;
};

// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewProjMat;
    matrix worldMat;
    matrix rotateMat;
    float3 cameraPos;
}

// 色
cbuffer ConstantBufferDataColor : register(b1)
{
    float4 color;
}

// UVWパラメーター
cbuffer ConstantBufferDataUVParameter : register(b2)
{
    float3 offset;
    float3 tiling;
};

cbuffer ConstantBufferDataFogParam : register(b3)
{
    uint stepCount;
    float stepLength;
    float density;

    float4 fogColor;
    float4 fogColorRate;
    float2 fogClamp;
}

cbuffer ConstantBufferObjectParam : register(b4)
{
    float3 objectPos;
    float3 objectScale;
}

// ライトグループ
static const uint directionalLightSize = 1;
static const uint pointLightSize = 20;
static const uint spotLightSize = 1;
cbuffer ConstantBufferDataLightGroup : register(b5)
{
    DirectionalLight directionalLight[directionalLightSize];
    PointLight pointLight[pointLightSize];
    SpotLight spotLight[spotLightSize];
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
    float3 wpos : POSITION;
};