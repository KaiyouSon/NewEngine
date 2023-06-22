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
cbuffer ConstBufferDataDOF : register(b2)
{
    float focusDepth;
    uint isRGB;
};

struct AppData
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct V2P
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};