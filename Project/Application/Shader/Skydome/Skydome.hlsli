// �}�e���A��
struct Material
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

// ���s����
struct DirectionalLight
{
    float4 color;
    float3 vec;
    uint isActive;
};

// �_����
struct PointLight
{
    float4 color;
    float3 pos;
    float radius;
    float3 colorRate;
    float decay;
    uint isActive;
};

// �X�|�b�g���C�g
struct SpotLight
{
    float4 color;
    float3 vec;
    float radius;
    float3 pos;
    float decay;
    float3 colorRate;
    uint isActive;
    float2 cosAngle;
};

// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewProjMat;
    matrix lightViewProjMat;
    matrix worldMat;
    float3 cameraPos;
    float3 lightCameraPos;
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

cbuffer ConstantBufferDataUVParameter : register(b4)
{
    float2 offset;
    float2 tiling;
};

cbuffer ConstantBufferDissolve : register(b5)
{
    float dissolve;
    float colorPower;
    float4 dissolveColor;
}

cbuffer ConstantBufferShadow : register(b6)
{
    uint isWriteShadow;
    float bias;
}

// ���C�g�O���[�v
static const uint directionalLightSize = 1;
static const uint pointLightSize = 5;
static const uint spotLightSize = 1;
cbuffer ConstantBufferDataLightGroup : register(b7)
{
    DirectionalLight directionalLight[directionalLightSize];
    PointLight pointLight[pointLightSize];
    SpotLight spotLight[spotLightSize];
};

cbuffer ConstantBufferDataDistanceFog : register(b8)
{
    float4 distanceFogColor;
    float2 fogNearFarDistance;
    float2 fogNearFarHeight;
    float3 distanceRate;
    uint isActiveDistanceFog;
    uint isActiveHeightFog;
};

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
    float4 spos : POSITIONT1;
};