#include "ObjectBasic.hlsli"

Texture2D<float4> tex : register(t0);	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv);
	return input.color;

	//float3 light = normalize(float3(1,-1,1));	// �E���� �����̃��C�g
	//float lightDiffuse = saturate(dot(-light, input.normal));
	//float3 shadeColor;
	//shadeColor = ambient;	// �A���r�G���g��
	//shadeColor += diffuse * lightDiffuse;
	//float4 texColor = tex.Sample(smp, input.uv);

	//return float4(texColor.rgb * shadeColor, texColor.a * alpha);
}