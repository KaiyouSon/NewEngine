// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewMat;
    matrix worldMat;
    matrix billboardMat;
}

// �F
cbuffer ConstantBufferDataColor : register(b1)
{
    float4 color; // �F
}

struct Appdata
{
    float4 pos : POSITION;
    float2 scale : TEXCOORD0;
    float rotY : TEXCOORD1;
};

struct V2G
{
    float4 pos : SV_POSITION; // �V�X�e���p���_���W
    float2 scale : TEXCOORD; // �X�P�[��
    float rotY : TEXCOORD1;
};

struct G2P
{
    float4 pos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
};