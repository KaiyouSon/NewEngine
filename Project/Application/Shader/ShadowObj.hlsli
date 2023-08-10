// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewProjMat;
    matrix worldMat;
}

static const int maxBones = 32;
cbuffer ConstantBufferDataSkinning : register(b1)
{
    matrix skinningMat[maxBones];
}

struct Appdata
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 wpos : POSITION0; // ���[���h���W
    float3 normal : NORMAL; // �@���x�N�g��
    float2 uv : TEXCOORD; // uv�l
};