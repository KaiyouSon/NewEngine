
// 3D変換行列
cbuffer ConstBufferDataTransform : register(b0)
{
	matrix mat; // 3D変換行列
}

// 色
cbuffer ConstBufferDataMaterial : register(b1)
{
    float4 color; // 色(RGBA)
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};
