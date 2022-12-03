#include "Particle.hlsli"

// �l�p�`�̒��_��
static const uint vnum = 4;

// �Z���^�[����̃I�t�Z�b�g
static const float4 offsetArray[vnum] =
{
	//float4(-0.5f,-0.5f,0,0),	// ����
	//float4(-0.5f,+0.5f,0,0),	// ����
	//float4(+0.5f,-0.5f,0,0),	// �E��
	//float4(+0.5f,+0.5f,0,0),	// �E��

	float4(-1.f,-1.f,0,0),	// ����
	float4(-1.f,+1.f,0,0),	// ����
	float4(+1.f,-1.f,0,0),	// �E��
	float4(+1.f,+1.f,0,0),	// �E��
};

// uv���蓖��
static const float2 uvArray[vnum] =
{
	float2(0,1),	// ����
	float2(0,0),	// ����
	float2(1,1),	// �E��
	float2(1,0),	// �E��
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output)
{
	GSOutput element;
	// ���_������
	for (uint i = 0; i < vnum; i++)
	{
		float4 offset = offsetArray[i] * input[0].scale;

		// ���[���h���W�x�[�X�ł��炷
		element.svpos = input[0].pos + offset;

		// �r���[�ϊ��A�ˉe�ϊ�
		element.svpos = mul(mat, element.svpos);
		element.uv = uvArray[i];
		element.color = input[0].color;
		output.Append(element);
	}
}