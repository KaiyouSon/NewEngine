#include "ObjectBasic.hlsli"

Texture2D<float4> tex : register(t0);	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	// �e�N�X�`���[�}�b�s���O
	float4 texColor = tex.Sample(smp, input.uv);

	// ����x
	const float shininess = 4.0f;
	// ���_���王�_�ւ̃x�N�g��
	float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
	// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
	float3 dotLightNormal = dot(lightVec, input.normal);
	// ���ˌ��x�N�g��
	float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);

	// �����ˌ�
	float3 tAmbient = ambient;
	// �g�U���ˌ�
	float3 tDiffuse = dotLightNormal * diffuse;
	// ���ʔ��ˌ�
	float tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

	// �V�F�[�f�B���O�ɂ��F
	float4 shaderColor;
	shaderColor.rgb = (tAmbient + tDiffuse + tSpecular) * lightColor;
	shaderColor.a = alpha;

	return shaderColor * texColor;



	//float4 texColor = tex.Sample(smp, input.uv);
	//return input.color * texColor;

	//float3 light = normalize(float3(1,-1,1));	// �E���� �����̃��C�g
	//float lightDiffuse = saturate(dot(-light, input.normal));
	//float3 shadeColor;
	//shadeColor = ambient;	// �A���r�G���g��
	//shadeColor += diffuse * lightDiffuse;
	//float4 texColor = tex.Sample(smp, input.uv);

	//return float4(texColor.rgb * shadeColor, texColor.a * alpha);
}