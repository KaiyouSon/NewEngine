#include "ToonRender.hlsli"
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

    // �e�̒��Ԓl
    const float middelValue = -0.35;

	// ���s����
    for (int i = 0; i < directionalLightNum; i++)
    {
        // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 lightNormal = dot(directionalLights[i].vec, vsOutput.normal);
        lightNormal = smoothstep(middelValue, middelValue, lightNormal) ? 1.f : 0.1f;
        
        float3 color = CalculateDirectionalLight(
            directionalLights[i], material, vsOutput.normal, lightNormal, eyeDir, shininess);
        shaderColor.rgb += color;
    }

    // �����v�Z�p�f�[�^
    LightCalculateData lightCalculateData;
    
	// �_����
    for (int i = 0; i < pointLightNum; i++)
    {
        // ���_�̃��[���h���W
        lightCalculateData.vertexPos = vsOutput.worldPos.xyz;
        // ���_�@��
        lightCalculateData.vertexNormal = vsOutput.normal;
        // ���_�̃��[���h���W���烉�C�g�Ɍ������x�N�g��
        lightCalculateData.lightVec = normalize(pointLights[i].pos - vsOutput.worldPos.xyz);
        // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 lightNormal = dot(lightCalculateData.lightVec, vsOutput.normal);
        lightCalculateData.lightNormal = smoothstep(middelValue, middelValue, lightNormal) ? 1.f : 0.1f;
        
        float3 color = CalculatePointLight(
            pointLights[i], material, lightCalculateData, eyeDir, shininess);
        shaderColor.rgb += color;
    }
  
	// �X�|�b�g���C�g
    for (int i = 0; i < spotLightNum; i++)
    {
        // ���_�̃��[���h���W
        lightCalculateData.vertexPos = vsOutput.worldPos.xyz;
        // ���_�@��
        lightCalculateData.vertexNormal = vsOutput.normal;
        // ���_�̃��[���h���W���烉�C�g�Ɍ������x�N�g��
        lightCalculateData.lightVec = normalize(spotLights[i].pos - vsOutput.worldPos.xyz);
        // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 lightNormal = dot(lightCalculateData.lightVec, vsOutput.normal);
        lightCalculateData.lightNormal = smoothstep(middelValue, middelValue, lightNormal) ? 1.f : 0.1f;
        
        float3 color = CalculateSpotLight(
            spotLights[i], material, lightCalculateData, eyeDir, shininess);
        shaderColor.rgb += color;
    }

	// �ۉe
    for (int i = 0; i < circleShadowNum; i++)
    {
        float3 color = CalculateCircleShadow(circleShadows[i], vsOutput.worldPos.xyz);
        shaderColor.rgb += color;
    }

    return shaderColor * texColor * color;
}