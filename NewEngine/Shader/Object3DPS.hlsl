#include "Object3DH.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

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

    float4 shaderColor = float4(float3(1, 1, 1) * tAmbient, alpha);

	// ���s����
    for (int i = 0; i < DirectionalLightNum; i++)
    {
        if (directionalLights[i].isActive == true)
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
        }
    }

	// �_����
    for (int i = 0; i < PointLightNum; i++)
    {
        if (pointLights[i].isActive == true)
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

	// �X�|�b�g���C�g
    for (int i = 0; i < spotLightNum; i++)
    {
        if (spotLights[i].isActive == true)
        {
			// ���C�g�ւ̕����x�N�g��
            float3 lightv = spotLights[i].pos - input.worldPos.xyz;
			// �x�N�g���̒���
            float d = length(lightv);
			// ���K��
            lightv = normalize(lightv);

			// ���������W��
            float atten = saturate(1.0f /
				(spotLights[i].atten.x +
					spotLights[i].atten.y * d +
					spotLights[i].atten.z * d * d));

			// �p�x����
            float cos = dot(lightv, spotLights[i].vec);
			// �����J�n�p�x����A�����I���p�x�ɂ����Č���
			// �����J�n�p�x�̓����͂P�{�A�����I���p�x�̊O���͂O�{�̋P�x
            float angleAtten = smoothstep(spotLights[i].factorAngleCos.y, spotLights[i].factorAngleCos.x, cos);
			// �p�x��������Z
            atten *= angleAtten;

			// ���C�g�����Ɍ������x�N�g���Ɩ@���̓���
            float3 dotLightNormal = dot(lightv, input.normal);

			// ���ˌ��x�N�g��
            float3 reflect = normalize(-lightv + 2 * dotLightNormal * input.normal);

			// �g�U���ˌ�
            float3 tDiffuse = dotLightNormal * diffuse;
			// ���ʔ��ˌ�
            float3 tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

			// �S�ĉ��Z����
            shaderColor.rgb += atten * (tDiffuse + tSpecular) * spotLights[i].color;
        }
    }

	// �ۉe
    for (int i = 0; i < circleShadowNum; i++)
    {
        if (circleShadows[i].isActive == true)
        {
			// �I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
            float3 casterV = circleShadows[i].pos - input.worldPos.xyz;

			// ���e�����ł̋���
            float d = dot(casterV, circleShadows[i].vec);

			// ���������W��
            float atten = saturate(1.0f /
				(circleShadows[i].atten.x +
					circleShadows[i].atten.y * d +
					circleShadows[i].atten.z * d * d));
			// �������}�C�i�X�Ȃ�O�ɂ���
            atten *= step(0, d);

			// ���z���C�g�̍��W
            float3 lightPos = circleShadows[i].pos + circleShadows[i].vec * circleShadows[i].disCasterLight;

			// �I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g��
            float3 lightV = normalize(lightPos - input.worldPos.xyz);

			// �p�x�v�Z
            float cos = dot(lightV, circleShadows[i].vec);
			// �����J�n�p�x����A�����I���p�x�ɂ����Č���
			// �����J�n�p�x�̓����͂P�{�A�����I���p�x�̊O���͂O�{�̋P�x
            float angleAtten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// �p�x��������Z
            atten *= angleAtten;

			// �S�Č��Z����
            shaderColor.rgb -= atten;
        }
    }

    if (isActiveFog == true)
    {
		// �J�����̍��W�ƒ��_�̋���
        float dis = distance(input.worldPos.xyz, cameraPos);
        float rate = smoothstep(fogNearDis, fogFarDis, dis);

		// �t�H�O�̐F
        float4 tFogColor = fogColor * rate;
        return shaderColor * texColor * color + tFogColor;
    }
    else
    {
        return shaderColor * texColor * color;
    }
}