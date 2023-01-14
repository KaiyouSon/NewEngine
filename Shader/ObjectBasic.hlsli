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
	float3 ambient : packoffset(c0);	// アンビエント係数
	float3 diffuse : packoffset(c1);	// ディフューズ係数
	float3 specular : packoffset(c2);	// スペキュラー係数
	float alpha : packoffset(c2.w);		// アルファ
}

// 色
cbuffer ConstantBufferDataColor : register(b2)
{
	float4 color;	// 色
}

static const int DirectionalLightNum = 3;

// 点光源
struct DirectionalLight
{
	float3 lightVec;		// 座標
	float3 color;	// 色
	uint isActive;
};

// ライト
cbuffer ConstantBufferDataDirectionalLight : register(b3)
{
	DirectionalLight directionalLights[DirectionalLightNum];
};

static const int PointLightNum = 3;

// 点光源
struct PointLight
{
	float3 pos;		// 座標
	float3 color;	// 色
	float3 atten;	// ライト距離減衰係数
	uint isActive;
};

cbuffer ConstantBufferDataPointLight : register(b4)
{
	PointLight pointLights[PointLightNum];
}


// 頂点シェーダーの出力構造体
// （頂点シェーダーからピクセルシェーダーヘのやり取りに使用する）
struct VSOutput
{
	float4 svpos : SV_POSITION;	// システム用頂点座標
	float4 worldPos : POSITION;	// ワールド座標
	float3 normal : NORMAL;		// 法線ベクトル
	//float4 color: COLOR;
	float2 uv : TEXCOORD;		// uv値
};