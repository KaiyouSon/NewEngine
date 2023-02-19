#include "Object3D.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutputSvposPosNormalUv vsOutput) : SV_TARGET
{
	// �e�N�X�`���[�}�b�s���O
    float4 texColor = tex.Sample(smp, vsOutput.uv);
    
	// ����x
    const float shininess = 4.0f;

	// ���_���王�_�ւ̃x�N�g��
    float3 eyeDir = normalize(cameraPos - vsOutput.worldPos.xyz);

    // �}�e���A��
    Material material = { ambient, diffuse, specular };
    
    // �V�F�[�_�[�J���[
    float4 shaderColor = float4(float3(1, 1, 1) * material.ambient, alpha);
    
	// ���s����
    for (int i = 0; i < directionalLightNum; i++)
    {
        float3 color = CalculateDirectionalLight(
            directionalLights[i], material, vsOutput.normal, eyeDir, shininess);
        shaderColor.rgb += color;
    }

	// �_����
    for (int i = 0; i < pointLightNum; i++)
    {
        float3 color = CalculatePointLight(
            pointLights[i], material, vsOutput.worldPos.xyz, vsOutput.normal, eyeDir, shininess);
        shaderColor.rgb += color;
    }

	// �X�|�b�g���C�g
    for (int i = 0; i < spotLightNum; i++)
    {
        float3 color = CalculateSpotLight(
            spotLights[i], material, vsOutput.worldPos.xyz, vsOutput.normal, eyeDir, shininess);
        shaderColor.rgb += color;
    }

	// �ۉe
    for (int i = 0; i < circleShadowNum; i++)
    {
        float3 color = CalculateCircleShadow(circleShadows[i], vsOutput.worldPos.xyz);
        shaderColor.rgb += color;
    }

    if (isActiveFog == true)
    {
		// �J�����̍��W�ƒ��_�̋���
        float dis = distance(vsOutput.worldPos.xyz, cameraPos);
        float rate = smoothstep(fogNearDis, fogFarDis, dis);

		// �t�H�O�̐F
        float4 tFogColor = fogColor * rate;
        float4 outPutColor = shaderColor * texColor * color;
        return fogColor * rate + outPutColor * (1 - rate);
    }
    else
    {
        return shaderColor * texColor * color;
    }
}