// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix wvpMat;
}

// �F
cbuffer ConstantBufferDataColor : register(b1)
{
    float4 color; // �F
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
};
