
// 3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b0)
{
	matrix mat; // 3D�ϊ��s��
}

// �F
cbuffer ConstBufferDataMaterial : register(b1)
{
	float4 color; // �F(RGBA)
};