#include "Lighting.hlsli"

// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewMat;
    matrix worldMat;
    float3 cameraPos;
}

// マテリアル
cbuffer ConstantBufferDataMaterial : register(b1)
{
    float3 ambient : packoffset(c0); // アンビエント係数
    float3 diffuse : packoffset(c1); // ディフューズ係数
    float3 specular : packoffset(c2); // スペキュラー係数
    float alpha : packoffset(c2.w); // アルファ
}

// 色
cbuffer ConstantBufferDataColor : register(b2)
{
    float4 color; // 色
}

static const int directionalLightNum = 1;
static const int pointLightNum = 3;
static const int spotLightNum = 3;
cbuffer ConstantBufferDataLightManager : register(b3)
{
    DirectionalLight directionalLights[directionalLightNum];
    PointLight pointLights[pointLightNum];
    SpotLight spotLights[spotLightNum];
};

// --- フォグ ---------------------- //
cbuffer ConstantBufferDataFog : register(b4)
{
    Fog fog;
};


static const int circleShadowNum = 1;
cbuffer ConstantBufferDataCircleShadow : register(b6)
{
    CircleShadow circleShadows[circleShadowNum];
}

static const int maxBones = 32;
cbuffer ConstantBufferDataSkinning : register(b5)
{
    matrix skinningMat[maxBones];
}

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};