
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

cbuffer ConstantBufferDataCircleGauge : register(b2)
{
    float startRadian;
    float endRadian;
}
