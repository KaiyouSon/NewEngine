#include "Ripple.hlsli"

// �}�e���A��
struct Material
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> dissolveTex : register(t1); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> shadowMapTex : register(t2); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(V2P i) : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
    // �~
    float len = distance(i.uv, float2(0.5f, 0.5f));
    
    // �~���O���ɍs���悤��
    float2 move = 0;
    move.x = cos(((len + offset.x) * 40.f) * 0.2);
    move.y = 0;
    
    // �e�N�X�`���[�}�b�s���O
    float4 texColor = tex.Sample(smp, i.uv + move);
    texColor.a = (1 - smoothstep(0.0f, 0.5f, len)) * 0.9f;
    
	// ����x
    const float shininess = 3.0f;
    float4 shainColor = color;
    shainColor.rgb *= shininess;
    
    return texColor * shainColor;
    


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
        float3 ambient = texColor.rgb * 0.6f * material.ambient.rgb;
     
        // �f�B�t���[�Y
        float intensity = saturate(dot(normalize(i.normal), dirLightVec));
        float4 diffuse = intensity * dirLightColor * float4(material.diffuse.rgb, 1);
    
        // �X�y�L�����[
        float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // ���_���王�_�ւ̃x�N�g��
        float3 reflectDir = -dirLightVec + 2 * i.normal * dot(i.normal, dirLightVec);
        float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
        adsColor.rgb = ambient + diffuse + specular * dirLightColor;
    }
    
    float4 resultColor = (adsColor * texColor * color);
    return resultColor;
}