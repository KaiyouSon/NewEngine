
cbuffer ConstBufferDataTransform : register(b0)
{
	matrix mat;// 3D�ϊ��s��
}

cbuffer ConstBufferDataB1 : register(b1)
{
	float3 ambient : packoffset(c0);	// �A���r�G���g�W��
	float3 diffuse : packoffset(c1);	// �f�B�t���[�Y�W��
	float3 specular : packoffset(c2);	// �X�y�L�����[�W��
	float alpha : packoffset(c2.w);	// �A���t�@
}
// �}�e���A��
//cbuffer ConstBufferDataMaterial : register(b0)
//{
//	float4 color; // �F(RGBA)
//};
//
//// 3D�ϊ��s��
//cbuffer ConstBufferDataTransform : register(b1)
//{
//	matrix mat; // 3D�ϊ��s��
//}

// ���_�V�F�[�_�[�̏o�͍\����
// �i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�w�̂����Ɏg�p����j
struct VSOutput
{
	float4 svpos : SV_POSITION;	// �V�X�e���p���_���W
	//float3 normal : NORMAL;		// �@���x�N�g��
	float4 color: COLOR;
	float2 uv : TEXCOORD;		// uv�l
};