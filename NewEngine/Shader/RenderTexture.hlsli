
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

// ���_�V�F�[�_�[����̏o�͍\����
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};