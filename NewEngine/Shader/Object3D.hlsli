#include "Lighting.hlsli"

// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
	//matrix mat;// 3D変換行列
    matrix viewMat;
    matrix worldMat;
    float3 cameraPos;
}

// マテリアル
cbuffer ConstantBufferDataMaterial : register(b1)
{
    float3 ambient : packoffset(c0); // アンビエント係数
    float3 diffuse : packoffset(c1); // ディフューズ係数
    float3 specular : packoffset(c2); // スペキュラー係数
    float alpha : packoffset(c2.w); // アルファ
}

// 色
cbuffer ConstantBufferDataColor : register(b2)
{
    float4 color; // 色
}

static const int directionalLightNum = 1;
static const int pointLightNum = 3;
static const int spotLightNum = 3;

cbuffer ConstantBufferDataLightManager : register(b3)
{
    DirectionalLight directionalLights[directionalLightNum];
    PointLight pointLights[pointLightNum];
    SpotLight spotLights[spotLightNum];
};

// --- フォグ ---------------------- //
cbuffer ConstantBufferDataFog : register(b4)
{
    uint isActiveFog;
    float fogNearDis;
    float fogFarDis;
    float4 fogColor;
};

// --- 丸影 ----------------------------------------------------- //
static const int circleShadowNum = 1;
//struct CircleShadow
//{
//    float3 vec; // 逆ベクトル
//    float3 pos; // 座標
//    float disCasterLight; // キャスターとライトの距離
//    float3 atten; // 距離減衰係数
//    float2 factorAngleCos; // 減衰角度のコサイン
//    uint isActive;
//};

cbuffer ConstantBufferDataCircleShadow : register(b6)
{
    CircleShadow circleShadows[circleShadowNum];
}