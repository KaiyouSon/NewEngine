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

// �F
cbuffer ConstBufferDataTransition : register(b2)
{
    float min;
    float max;
};

// �F
cbuffer ConstBufferDataUV : register(b3)
{
    float2 offset;
    float2 tiling;
};

struct Appdata
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};