
// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
	matrix mat; // 3D�ϊ��s��
	//matrix viewMat;
	//matrix worldMat;
	//float3 cameraPos;
}

// �}�e���A��
cbuffer ConstantBufferDataMaterial : register(b1)
{
	float4 color; // �F(RGBA)
};

//// ���C�g
//cbuffer ConstBufferDataLight : register(b2)
//{
//	float3 lightVec;	// ���C�g�ւ̕����̒P�ʃx�N�g��
//	float3 lightColor;	// ���C�g�̐F(RGB)
//};

// ���_�V�F�[�_�[�̏o�͍\����
// �i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�w�̂����Ɏg�p����j
struct VSOutput
{
	float4 svpos : SV_POSITION;	// �V�X�e���p���_���W
	float3 normal : NORMAL;		// �@���x�N�g��
	float2 uv : TEXCOORD;		// uv�l
};