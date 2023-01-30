
// 3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b0)
{
	matrix mat; // 3D�ϊ��s��
}

// �}�e���A��
cbuffer ConstBufferDataMaterial : register(b1)
{
	float4 color; // �F(RGBA)
};

// ���_�V�F�[�_�[�̏o�͍\����
// �i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�w�̂����Ɏg�p����j
struct VSOutput
{
	float4 svpos : SV_POSITION;	// �V�X�e���p���_���W
	float2 uv : TEXCOORD;		// uv�l
};