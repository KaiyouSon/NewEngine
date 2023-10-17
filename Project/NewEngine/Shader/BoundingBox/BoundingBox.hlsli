// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewProjMat;
    matrix worldMat;
    float3 cameraPos;
}

// �F
cbuffer ConstantBufferDataColor : register(b1)
{
    float4 color; // �F
}

// UVW�p�����[�^�[
cbuffer ConstantBufferDataUVParameter : register(b2)
{
    float2 offset;
    float2 tiling;
};

cbuffer ConstantBufferDataScreenToWorldMat : register(b3)
{
    uint stepCount;
    float stepLength;
    float2 smoothingClamp;
    float4 fogColor;
    float4 fogColorRate;
}

struct Appdata
{
    float4 pos : POSITION;
    float3 uvw : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float3 uvw : TEXCOORD; // uv�l
    float3 wpos : POSITION;
};