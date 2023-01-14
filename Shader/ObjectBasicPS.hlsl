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

	// �����ˌ�
	float3 tAmbient = ambient;

	float4 shaderColor = float4(float3(1,1,1) * tAmbient, alpha);

	// ���s����
	for (int i = 0; i < DirectionalLightNum; i++)
	{
		if (directionalLights[i].isActive)
		{
			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotLightNormal = dot(directionalLights[i].lightVec, input.normal);
			// ���ˌ��x�N�g��
			float3 reflect = normalize(-directionalLights[i].lightVec + 2 * dotLightNormal * input.normal);

			// �g�U���ˌ�
			float3 tDiffuse = dotLightNormal * diffuse;
			// ���ʔ��ˌ�
			float tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

			// �S�ĉ��Z����
			shaderColor.rgb += (tDiffuse + tSpecular) * directionalLights[i].color;

			// �V�F�[�f�B���O�ɂ��F
			//shaderColor.rgb += (tAmbient + tDiffuse + tSpecular) * color;
			//shaderColor.a = alpha;
		}
	}

	// �_����
	for (int i = 0; i < PointLightNum; i++)
	{
		if (pointLights[i].isActive)
		{
			// ���C�g�ւ̕����x�N�g��
			float3 lightv = pointLights[i].pos - input.worldPos.xyz;
			// �x�N�g���̒���
			float d = length(lightv);
			// ���K��
			lightv = normalize(lightv);

			// ���������W��
			float atten = 1.0f /
				(pointLights[i].atten.x +
				pointLights[i].atten.y * d +
				pointLights[i].atten.z * d * d);

			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(lightv, input.normal);

			// ���ˌ��x�N�g��
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// �g�U���ˌ�
			float3 tDiffuse = dotlightnormal * diffuse;
			// ���ʔ��ˌ�
			float3 tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

			// �S�ĉ��Z����
			shaderColor.rgb += atten * (tDiffuse + tSpecular) * pointLights[i].color;
		}
	}

	return shaderColor * texColor * color;
}