#include "Grass.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> shadowMapTex : register(t1);
SamplerState smp : register(s0);

float CalcShadow(float4 spos)
{
    float shadow = 1;
    
    float2 shadowTexUV = spos.xy / spos.w;
    shadowTexUV *= float2(0.5f, -0.5f);
    shadowTexUV += 0.5f;
        
    // ���C�g�r���[�ł̃X�N���[����Ԃł�z�l���v�Z����
    float z = spos.z / spos.w;
        
    if (shadowTexUV.x > 0.01f && shadowTexUV.x < 0.99f &&
        shadowTexUV.y > 0.01f && shadowTexUV.y < 0.99f)
    {
        float shadowDepth = shadowMapTex.Sample(smp, shadowTexUV).r;
        if (shadowDepth + 0.0001f < z)
        {
            shadow *= 0.3f;
        }
    }
    
    return shadow;
}

float4 CalcLighting(G2P i)
{
    Material material;
    material.ambient = ambient;
    material.diffuse = diffuse;
    material.specular = specular;
    
    // ����x
    const float shininess = 2.0f;
    
    float4 adsColor = 0;
    
    uint index = 0;
    // ���s�����̌v�Z
    for (index = 0; index < directionalLightSize; index++)
    {
        if (directionalLight[index].isActive == true)
        {
            // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float dotLightNormal = dot(directionalLight[index].vec, i.normal);
        
            // �A���r�G���g
            float3 ambient = material.ambient.rgb + float3(0.2f, 0.2f, 0.2f);
     
            // �f�B�t���[�Y
            float3 diffuse = dotLightNormal * material.diffuse.rgb;
    
            // �X�y�L�����[
            float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // ���_���王�_�ւ̃x�N�g��
            float3 reflectDir = normalize(-directionalLight[index].vec + 2 * dotLightNormal * i.normal);
            float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
            adsColor.rgb += (ambient + diffuse + specular) * directionalLight[index].color.rgb;
        }
    }

    // �_�����̌v�Z
    for (index = 0; index < pointLightSize; index++)
    {
        if (pointLight[index].isActive == true)
        {
            // ���C�g�w�̃x�N�g��
            float3 lightVec = normalize(pointLight[index].pos - i.wpos.xyz);
            float d = distance(pointLight[index].pos, i.wpos.xyz);
            
            float s = d / pointLight[index].radius;
            if (s >= 1.0)
            {
                continue;
            }
            
            float s2 = s * s;
            
            float atten = pointLight[index].decay * ((1 - s2) * (1 - s2));
            
            // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotLightNormal = dot(lightVec, i.normal);
            
            // �f�B�t���[�Y
            float3 diffuse = saturate(dotLightNormal * material.diffuse.rgb);
    
            // �X�y�L�����[
            float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // ���_���王�_�ւ̃x�N�g��
            float3 reflectDir = normalize(-lightVec + 2 * dotLightNormal * i.normal);
            float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
            adsColor.rgb += atten * (diffuse + specular) *
                            pointLight[index].color.rgb * pointLight[index].colorRate.rgb;
        }
    }
    
    // �X�|�b�g���C�g�̌v�Z
    for (index = 0; index < spotLightSize; index++)
    {
        if (spotLight[index].isActive == true)
        {
            // ���C�g�w�̃x�N�g��
            float3 lightVec = normalize(spotLight[index].pos - i.wpos.xyz);
            float d = distance(spotLight[index].pos, i.wpos.xyz);
            
            float s = d / spotLight[index].radius;
            if (s >= 1.0)
            {
                continue;
            }
            
            float s2 = s * s;
            
            float cosAngle = dot(lightVec, spotLight[index].vec);
            float falloffFactor = saturate((cosAngle - spotLight[index].cosAngle.y) / (spotLight[index].cosAngle.x - spotLight[index].cosAngle.y));
            
            float atten = spotLight[index].decay * ((1 - s2) * (1 - s2)); // / (1 + falloffFactor * s);
            atten *= falloffFactor;
            
            // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotLightNormal = dot(lightVec, i.normal);
            
            // �f�B�t���[�Y
            float3 diffuse = saturate(dotLightNormal * material.diffuse.rgb);
    
            // �X�y�L�����[
            float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // ���_���王�_�ւ̃x�N�g��
            float3 reflectDir = normalize(-lightVec + 2 * dotLightNormal * i.normal);
            float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
            adsColor.rgb += atten * (diffuse + specular) *
                        spotLight[index].color.rgb * spotLight[index].colorRate.rgb;
        }
    }
    
    adsColor.a = 1;
    return adsColor;
}

float4 main(G2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);
    
    clip(texColor.a - 0.5);
    
    float4 adsColor = CalcLighting(i);
    
    float shadow = 1.0f;
    shadow = CalcShadow(i.spos);
    
    float4 resultColor = texColor * adsColor * color;
    resultColor.rgb *= shadow;
    
    return resultColor;
}
