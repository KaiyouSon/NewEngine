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

struct Appdata
{
    float4 pos : POSITION;
    float3 uvw : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float3 uvw : TEXCOORD; // uv�l
};