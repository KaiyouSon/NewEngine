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
    float4 color;
}

// UVW�p�����[�^�[
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