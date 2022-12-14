// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
	//matrix mat;// 3D�ϊ��s��
	matrix viewMat;
	matrix worldMat;
	float3 cameraPos;
}

// �}�e���A��
cbuffer ConstantBufferDataMaterial : register(b1)
{
	float3 ambient : packoffset(c0);	// �A���r�G���g�W��
	float3 diffuse : packoffset(c1);	// �f�B�t���[�Y�W��
	float3 specular : packoffset(c2);	// �X�y�L�����[�W��
	float alpha : packoffset(c2.w);	// �A���t�@
}

// ���C�g
cbuffer ConstantBufferDataLight : register(b2)
{
	float3 lightVec;	// ���C�g�ւ̕����̒P�ʃx�N�g��
	float3 lightColor;	// ���C�g�̐F(RGB)
};

// �_����
struct PointLight
{
	float3 pos;		// ���W
	float3 color;	// �F
	float4 atten;	// ���C�g���������W��
	uint active;
};

// ���_�V�F�[�_�[�̏o�͍\����
// �i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�w�̂����Ɏg�p����j
struct VSOutput
{
	float4 svpos : SV_POSITION;	// �V�X�e���p���_���W
	float4 worldPos : POSITION;	// ���[���h���W
	float3 normal : NORMAL;		// �@���x�N�g��
	//float4 color: COLOR;
	float2 uv : TEXCOORD;		// uv�l
};