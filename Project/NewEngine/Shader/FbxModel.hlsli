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

static const int maxBones = 32;
cbuffer ConstantBufferDataSkinning : register(b3)
{
    matrix skinningMat[maxBones];
}

struct SkinOutput
{
    float4 pos;
    float3 normal;
};