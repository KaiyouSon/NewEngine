
// 3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b0)
{
	matrix mat; // 3D�ϊ��s��
}

// ���_�V�F�[�_�[�̏o�͍\����
// �i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�w�̂����Ɏg�p����j
struct VSOutput
{
	float4 pos : POSITION;	// �V�X�e���p���_���W
	float scale : TEXCOORD;	// �X�P�[��
	float4 color : COLOR;
};

// �W�I���g���V�F�[�_�[�ւ̏o��
struct GSOutput
{
	float4 svpos : SV_POSITION;	// �V�X�e���p���_���W
	float2 uv : TEXCOORD;		// uv�l
	float4 color : COLOR;
};