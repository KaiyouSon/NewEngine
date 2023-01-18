// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
	matrix viewMat;
	matrix worldMat;
	float3 cameraPos;
}

// �F
cbuffer ConstantBufferDataColor : register(b1)
{
	float4 color;	// �F
}

// ���_�V�F�[�_�[�̏o�͍\����
struct VSOutput
{
	float4 svpos : SV_POSITION;	// �V�X�e���p���_���W
	float4 worldPos : POSITION;	// ���[���h���W
	float3 normal : NORMAL;		// �@���x�N�g��
	float2 uv : TEXCOORD;		// uv�l
};