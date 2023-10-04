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

// UV���
cbuffer ConstantBufferDataUVParameter : register(b2)
{
    float2 offset;
    float2 tiling;
};

struct Appdata
{
    float4 pos : POSITION;
    float2 scale : TEXCOORD0;
    float rot : TEXCOORD1;
    float shininess : TEXCOORD2;
    float4 color : COLOR;
};

struct V2G
{
    float4 pos : SV_POSITION; // �V�X�e���p���_���W
    float2 scale : TEXCOORD0; // �X�P�[��
    float rot : TEXCOORD1;    // ��]
    float shininess : TEXCOORD2;
    float4 color : COLOR; //�F
};

struct G2P
{
    float4 pos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
    float shininess : TEXCOORD2;
    float4 color : COLOR; //�F
};

