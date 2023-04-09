#include "Lighting.hlsli"

// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewMat;
    matrix worldMat;
    float3 cameraPos;
}

// �}�e���A��
cbuffer ConstantBufferDataMaterial : register(b1)
{
    float3 ambient : packoffset(c0); // �A���r�G���g�W��
    float3 diffuse : packoffset(c1); // �f�B�t���[�Y�W��
    float3 specular : packoffset(c2); // �X�y�L�����[�W��
    float alpha : packoffset(c2.w); // �A���t�@
}

// �F
cbuffer ConstantBufferDataColor : register(b2)
{
    float4 color; // �F
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

// --- �t�H�O ---------------------- //
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