
// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewMat;
    matrix worldMat;
    float3 cameraPos;
}

// �}�e���A��
cbuffer ConstantBufferDataMaterial : register(b1)
{
    float3 ambient : packoffset(c0); // �A���r�G���g�W��
    float3 diffuse : packoffset(c1); // �f�B�t���[�Y�W��
    float3 specular : packoffset(c2); // �X�y�L�����[�W��
    float alpha : packoffset(c2.w); // �A���t�@
}

// �F
cbuffer ConstantBufferDataColor : register(b2)
{
    float4 color; // �F
}

static const int maxBones = 32;
cbuffer ConstantBufferDataSkinning : register(b3)
{
    matrix skinningMat[maxBones];
}

// --- �t�H�O ---------------------- //
cbuffer ConstantBufferDataUVParameter : register(b4)
{
    float2 offset;
    float2 tiling;
};

// ���s����
cbuffer ConstantBufferDirectionalLight : register(b5)
{
    float4 dirLightColor; // �F
    float3 dirLightVec; // ����
    uint isActiveDirLight;
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
    float4 worldPos : POSITION; // ���[���h���W
    float2 uv : TEXCOORD; // uv�l
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};