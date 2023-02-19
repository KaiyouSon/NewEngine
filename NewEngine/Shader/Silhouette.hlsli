// 3D•ÏŠ·s—ñ
cbuffer ConstantBufferDataTransform : register(b0)
{
	matrix viewMat;
	matrix worldMat;
	float3 cameraPos;
}

// F
cbuffer ConstantBufferDataColor : register(b1)
{
	float4 color;	// F
}