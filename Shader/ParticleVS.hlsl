#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float scale : TEXCOORD, float4 color : COLOR)
{
	// �s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	output.pos = pos;
	output.scale = scale;
	output.color = color;
	return output;
}