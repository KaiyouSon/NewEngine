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
    float2 size = float2(0.3, 1);
    // �s�N�Z���̍��W�𐳋K�����ĕH�`�̒��S�����_�ɂ���
    float2 centerUV = i.uv * 2.0f - 1.0f;
    float2 newUV = i.uv * tiling;
    
    // ���E�ɗh�炷����
    float amplitude = 0.15f; // �U��
    float frequency = 2; // ���g��
    float offsetX = sin(offset.x * frequency + i.uv.x * frequency) * amplitude;
    
    // �H�`�̕�����
    float disToCenter = abs(centerUV.x / size.y) + abs((centerUV.y + offsetX) / size.x);
    
    // uv����Ɉړ�
    newUV.x += offset.x * 0.25f;

    // �e�N�X�`���[�}�b�s���O
    float4 texColor = tex.Sample(smp, newUV);
    
    // �H�`���̃A���t�@
    texColor.a = smoothstep(0.01f, 0.3f, texColor.r) * texColor.a;
    
    // �H�`�̘g�̃A���t�@
    float rate = 1 - smoothstep(0.5f, 1.0f, disToCenter);
    texColor.a *= rate;
    
    clip(texColor.a - 0.1f);
    
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