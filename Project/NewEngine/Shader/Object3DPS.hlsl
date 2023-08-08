#include "Object3D.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> dissolveTex : register(t1); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> shadowMapTex : register(t2); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutput main(V2P i)// : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
	// �e�N�X�`���[�}�b�s���O
    float4 texColor = tex.Sample(smp, newUV);
    float4 mask = dissolveTex.Sample(smp, newUV);
    
    float maskIntensity = smoothstep(0.1, 0.25, (0.5f + mask.r) - dissolve);
    clip(maskIntensity - 0.01f);
    
	// ����x
    const float shininess = 4.0f;

    // �}�e���A��
    Material material = { ambient, diffuse, specular };
    
    // �V�F�[�_�[�J���[
    float4 shaderColor = 0;
    
    float4 adsColor = float4(0, 0, 0, 1);
    if (isActiveDirLight == true)
    {
        // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float dotLightNormal = dot(dirLightVec, i.normal);
        
        // �A���r�G���g
        float3 ambient = /*texColor.rgb * 0.1f **/material.ambient.rgb;
     
        // �f�B�t���[�Y
        float intensity = saturate(dot(normalize(i.normal), dirLightVec));
        float4 diffuse = intensity * dirLightColor * float4(material.diffuse.rgb, 1);
    
        // �X�y�L�����[
        float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // ���_���王�_�ւ̃x�N�g��
        float3 reflectDir = -dirLightVec + 2 * i.normal * dot(i.normal, dirLightVec);
        float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
        adsColor.rgb = ambient + diffuse + specular * dirLightColor;
    }
    
    float shadow = 1.0f;
    //�V���h�E�}�b�v��Z�l���Q��
    float w = 1.0f / i.spos.w;
    float2 shadowTexUV;
    shadowTexUV.x = (1.0f + i.spos.x * w) * 0.5f;
    shadowTexUV.y = (1.0f - i.spos.y * w) * 0.5f;
		//uv���W��0�`1�Ȃ�e���������
    if (shadowTexUV.x >= 0 && shadowTexUV.x <= 1.0f && shadowTexUV.y >= 0 && shadowTexUV.y <= 1.0f)
    {
        if (shadowMapTex.Sample(smp, shadowTexUV).x + 0.0005f < i.spos.z * w)
        {
            shadow *= 0.5f;
        }
    }
    
    float4 resultColor = (adsColor * texColor * color);
    resultColor.rgb *= shadow;
    
    PSOutput output;
    output.target0 = resultColor * maskIntensity + dissolveColor * colorPower * (1 - maskIntensity);
    //output.target0 = float4(shadow, shadow, shadow, 1);
    output.target1 = float4(1 - resultColor.rgb, color.a);
    return output;
    
    //return shaderColor * texColor * color;
}