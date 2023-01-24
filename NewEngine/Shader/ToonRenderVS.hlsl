#include "ToonRenderH.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
	float4 wnormal = normalize(mul(worldMat, float4(normal, 0)));
	float4 wpos = mul(worldMat, pos);

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mul(viewMat, worldMat), pos);
	output.worldPos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;

	return output;
}