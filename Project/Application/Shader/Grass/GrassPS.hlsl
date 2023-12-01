#include "Grass.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> shadowMapTex : register(t1);
SamplerState smp : register(s0);

// POM�̌v�Z
float3 CalcParallaxMapping(G2P i);

// ���C�e�B���O�̌v�Z
float4 CalcLighting(G2P i);

// �A�̌v�Z
float CalcShadow(float4 spos);

float4 main(G2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);
    float4 resultColor = texColor;
    
    // POM
    float4 pomColor = float4(CalcParallaxMapping(i).rgb, 1);
    resultColor *= pomColor * color;
    
    clip(resultColor.a - 0.75);
    //return resultColor;
    
    // ���C�e�B���O
    float4 adsColor = CalcLighting(i);
    resultColor *= adsColor;
    
    // �V���h�E
    float shadow = 1.0f;
    shadow = CalcShadow(i.spos);
    resultColor.rgb *= shadow;
    
    
    return resultColor;
}

float3 CalcParallaxMapping(G2P i)
{
    float2 uv = i.uv;
    
    float height = tex.Sample(smp, i.uv).r;
    float3 eyeDir = normalize(i.wpos.xyz - cameraPos); // ���_���王�_�ւ̃x�N�g��
    float2 parallaxOffset = /* heightScale **/eyeDir.xy * height;
    //uv += parallaxOffset/* + float2(0.5f, 0.5f)*/;
    
    // ���C�}�[�`���O
    float3 resultColor = tex.Sample(smp, uv);
    float stepSize = heightScale;
    float maxStep = 20;
    
    for (float i = 0; i < maxStep; i++)
    {
         // ���C�̐�ɂ���|�C���g�̍������擾
        float currentHeight = tex.Sample(smp, uv).r;

        // ���C�̏Փ˔���
        //if (uv.y > currentHeight)
        //{
            // �T���v�����O
        float4 sampleColor = tex.Sample(smp, uv);
        resultColor += sampleColor.rgb;

            // �e�N�X�`�����W���X�V
            //uv += parallaxOffset /*+ float2(0.5f, 0.5f)*/;
            
            // �X�e�b�v�T�C�Y�����i�߂�
        uv += stepSize * eyeDir.xy;
        //}
        //else
        //{
        //    break; // �Փ˂����烋�[�v�I��
        //}
    }
    
    return resultColor / maxStep;
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
            float3 halfVector = normalize(-directionalLight[index].vec + eyeDir);
            float nDoth = dot(normalize(i.normal), halfVector);
            float3 specular = pow(saturate(nDoth), shininess) * material.specular.rgb;
    
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


float CalcShadow(float4 spos)
{
    float shadow = 1;
    
    // ���C�g�r���[�ł̃X�N���[����Ԃł�z�l���v�Z����
    float z = spos.z / spos.w;
    
    // �V���h�E�}�b�v��UV���Z�o
    float2 shadowTexUV = spos.xy / spos.w;
    shadowTexUV *= float2(0.5f, -0.5f);
    shadowTexUV += 0.5f;
    
    //if (shadowTexUV.x > 0.01f && shadowTexUV.x < 0.99f &&
    //    shadowTexUV.y > 0.01f && shadowTexUV.y < 0.99f)
    //{
    //    float shadowDepth = shadowMapTex.Sample(smp, shadowTexUV).r;
    //    if (shadowDepth + 0.0001f < z)
    //    {
    //        shadow *= 0.5f;
    //    }
    //}

    //return shadow;
    
    float shadowFactor = 0;
    float shiftNum = 3;
    float shiftWidth = 0.00005f;
    float count = 0;
    [unroll]
    for (float py = -shiftNum / 2; py <= shiftNum / 2; py++)
    {
        [unroll]
        for (float px = -shiftNum / 2; px <= shiftNum / 2; px++)
        {
		    // �F�擾����UV���W
            float2 offset = float2(px, py);
            float2 pickUV = shadowTexUV + offset * shiftWidth;
            
            // ��ʊO�̐F���擾���Ȃ��悤��
            pickUV = clamp(pickUV, 0.001, 0.999);

            // �V���h�E �}�b�v����[�x���T���v�����O
            float shadowDepth = shadowMapTex.Sample(smp, pickUV).r;
            if (shadowDepth + 0.0005f < z)
            {
                shadowFactor += 0.75f;
            }
            count++;
        }
    }

    // �T���v�����Ŋ����Đ��K��
    shadow = 1 - shadowFactor / count;
    return shadow;
}

