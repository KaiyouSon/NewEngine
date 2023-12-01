// マテリアル
struct Material
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
};


// 平行光源
struct DirectionalLight
{
    float4 color;
    float3 vec;
    uint isActive;
};

// 点光源
struct PointLight
{
    float4 color;
    float3 pos;
    float radius;
    float3 colorRate;
    float decay;
    uint isActive;
};

// スポットライト
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

// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewProjMat;
    matrix lightViewProjMat;
    matrix worldMat;
    matrix billboardMat;
    float3 cameraPos;
    float3 lightCameraPos;
}

// 色
cbuffer ConstantBufferDataColor : register(b1)
{
    float4 color; // 色
}

// マテリアル
cbuffer ConstantBufferDataMaterial : register(b2)
{
    float3 ambient : packoffset(c0); // アンビエント係数
    float3 diffuse : packoffset(c1); // ディフューズ係数
    float3 specular : packoffset(c2); // スペキュラー係数
    float alpha : packoffset(c2.w); // アルファ
}

// POM
cbuffer ConstantBufferDataPOM : register(b3)
{
    float heightScale; // 色
}

// ライトグループ
static const uint directionalLightSize = 1;
static const uint pointLightSize = 5;
static const uint spotLightSize = 1;
cbuffer ConstantBufferDataLightGroup : register(b4)
{
    DirectionalLight directionalLight[directionalLightSize];
    PointLight pointLight[pointLightSize];
    SpotLight spotLight[spotLightSize];
};

struct Appdata
{
    float4 pos : POSITION;
    float2 scale : TEXCOORD0;
    float2 timer : TEXCOORD1;
};

struct V2G
{
    float4 pos : SV_POSITION; // システム用頂点座標
    float2 scale : TEXCOORD0; // スケール
    float2 timer : TEXCOORD1;
};

struct G2P
{
    float4 pos : SV_POSITION; //システム用頂点座標
    float2 uv : TEXCOORD; //uv値
    float3 normal : NORMAL;
    float4 wpos : POSITION0; // ワールド座標
    float4 spos : POSITIONT1;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};
