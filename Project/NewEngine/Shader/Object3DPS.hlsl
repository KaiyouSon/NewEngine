#include "Object3D.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutput main(V2P i)// : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
	// �e�N�X�`���[�}�b�s���O
    float4 texColor = tex.Sample(smp, newUV);
    
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
        //float intensity = saturate(dot(normalize(i.normal), dirLightVec));
        //float4 diffuse = intensity * dirLightColor * float4(material.diffuse.rgb, 1);
        float3 diffuse = dotLightNormal * material.diffuse.rgb;
    
        // �X�y�L�����[
        float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // ���_���王�_�ւ̃x�N�g��
        float3 reflectDir = -dirLightVec + 2 * i.normal * dot(i.normal, dirLightVec);
        float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
        adsColor.rgb = ambient + diffuse + specular * dirLightColor;
    }

    float4 resultColor = adsColor * texColor * color;
    
    PSOutput output;
    output.target0 = resultColor;
    output.target1 = float4(1 - resultColor.rgb, color.a);
    return output;
    
    //return shaderColor * texColor * color;
}