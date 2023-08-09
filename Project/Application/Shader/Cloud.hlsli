
// 3D変換行列
cbuffer ConstantBufferDataTransform : register(b0)
{
    matrix viewMat;
    matrix lightViewMat;
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

static const int maxBones = 32;
cbuffer ConstantBufferDataSkinning : register(b3)
{
    matrix skinningMat[maxBones];
}

// --- フォグ ---------------------- //
cbuffer ConstantBufferDataUVParameter : register(b4)
{
    float2 offset;
    float2 tiling;
};

// 平行光源
cbuffer ConstantBufferDirectionalLight : register(b5)
{
    float4 dirLightColor; // 色
    float3 dirLightVec; // 方向
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
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 worldPos : POSITION; // ワールド座標
    float2 uv : TEXCOORD; // uv値
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};