
cbuffer cbuff0 : register(b0)
{
};

cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat; // 3D�ϊ��s��
}

// ���_�V�F�[�_�[����̏o�͍\����
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};