// 3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b0)
{
    matrix mat; // 3D�ϊ��s��
}

// �F
cbuffer ConstBufferDataMaterial : register(b1)
{
    float4 color; // �F(RGBA)
};

// �e
cbuffer ConstBufferDataTransformShadowMap : register(b2)
{
    matrix viewProjMat;
    matrix worldMat;
};

struct Appdata
{
    float4 pos : POSITION0;
    float2 uv : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION;
    float4 spos : POSITION;
    float2 uv : TEXCOORD;
};
