#include "Object3D.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> dissolveTex : register(t1); // 0番スロットに設定されたテクスチャ
Texture2D<float4> shadowMapTex : register(t2); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 CalcShadowColor(V2P i, float4 color)
{
    float4 resultColor = color;
    
    // ライトビュースクリーン空間からUV空間に座標変換
    float2 shadowTexUV = i.spos.xy / i.spos.w;
    shadowTexUV *= float2(0.5f, -0.5f);
    shadowTexUV += 0.5f;
        
    // ライトビューでのスクリーン空間でのz値を計算する
    float z = i.spos.z;
    float w = 1.0f / i.spos.w;
    
    if (shadowTexUV.x > 0.01f && shadowTexUV.x < 0.99f &&
        shadowTexUV.y > 0.01f && shadowTexUV.y < 0.99f)
    {
        float shadowValue = shadowMapTex.Sample(smp, shadowTexUV).r;
        
        if (shadowValue + bias < z * w /* && z <= 1.0f*/)
        {
            float depthSq = shadowValue * shadowValue;
            
            float variance = min(max(shadowValue - depthSq, 0.0001f), 1.0f);
            
            float md = z - shadowValue.x;
            
            float lightFactor = variance / (variance + md * md);
            
            float3 shadowColor = resultColor.rgb * 0.5f;
            
            shadowColor.rgb = lerp(shadowColor, color.xyz, lightFactor);
        }
    }
    
    return resultColor;
}

float CalcShadow(float4 spos)
{
    float shadow = 1;
    
    float2 shadowTexUV = spos.xy / spos.w;
    shadowTexUV *= float2(0.5f, -0.5f);
    shadowTexUV += 0.5f;
        
    // ライトビューでのスクリーン空間でのz値を計算する
    float z = spos.z / spos.w;
        
    if (shadowTexUV.x > 0.01f && shadowTexUV.x < 0.99f &&
        shadowTexUV.y > 0.01f && shadowTexUV.y < 0.99f)
    {
        float shadowDepth = shadowMapTex.Sample(smp, shadowTexUV).r;
        if (shadowDepth + bias < z)
        {
            shadow *= 0.25f;
        }
    }
    
    return shadow;
}

PSOutput main(V2P i)// : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
	// テクスチャーマッピング
    float4 texColor = tex.Sample(smp, newUV);
    float4 mask = dissolveTex.Sample(smp, newUV);
    
    float maskIntensity = smoothstep(0.1, 0.25, (0.5f + mask.r) - dissolve);
    clip(maskIntensity - 0.01f);
    
	// 光沢度
    const float shininess = 2.0f;

    // マテリアル
    Material material = { ambient, diffuse, specular };
    
    // シェーダーカラー
    float4 shaderColor = 0;
    
    float4 adsColor = float4(0, 0, 0, 1);
    
    uint index = 0;
    // 平行光源の計算
    for (index = 0; index < directionalLightSize; index++)
    {
        if (directionalLight[index].isActive == true)
        {
            // ライトに向かうベクトルと法線の内積
            float dotLightNormal = dot(directionalLight[index].vec, i.normal);
        
            // アンビエント
            float3 ambient = material.ambient.rgb + float3(0.2f, 0.2f, 0.2f);
     
            // ディフューズ
            float3 diffuse = dotLightNormal * material.diffuse.rgb;
    
            // スペキュラー
            float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
            float3 reflectDir = normalize(-directionalLight[index].vec + 2 * dotLightNormal * i.normal);
            float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
            adsColor.rgb += (ambient + diffuse + specular) * directionalLight[index].color.rgb;
        }
    }

    // 点光源の計算
    for (index = 0; index < pointLightSize; index++)
    {
        if (pointLight[index].isActive == true)
        {
            // ライトヘのベクトル
            float3 lightVec = normalize(pointLight[index].pos - i.wpos.xyz);
            float d = distance(pointLight[index].pos, i.wpos.xyz);
            
            float s = d / pointLight[index].radius;
            if (s >= 1.0)
            {
                continue;
            }
            
            float s2 = s * s;
            
            float atten = pointLight[index].decay * ((1 - s2) * (1 - s2));
            
            // ライトに向かうベクトルと法線の内積
            float3 dotLightNormal = dot(lightVec, i.normal);
            
            // ディフューズ
            float3 diffuse = saturate(dotLightNormal * material.diffuse.rgb);
    
            // スペキュラー
            float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
            float3 reflectDir = normalize(-lightVec + 2 * dotLightNormal * i.normal);
            float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
            adsColor.rgb += atten * (diffuse + specular) *
                            pointLight[index].color.rgb * pointLight[index].colorRate.rgb;
        }
    }
    
    // スポットライトの計算
    for (index = 0; index < spotLightSize; index++)
    {
        if (spotLight[index].isActive == true)
        {
            // ライトヘのベクトル
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
            
            // ライトに向かうベクトルと法線の内積
            float3 dotLightNormal = dot(lightVec, i.normal);
            
            // ディフューズ
            float3 diffuse = saturate(dotLightNormal * material.diffuse.rgb);
    
            // スペキュラー
            float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
            float3 reflectDir = normalize(-lightVec + 2 * dotLightNormal * i.normal);
            float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
            adsColor.rgb += atten * (diffuse + specular) *
                            spotLight[index].color.rgb * spotLight[index].colorRate.rgb;
        }
    }
    
    float4 resultColor = (adsColor * texColor * color);
    
    
    float shadow = 1.0f;
   //float4 shadowColor = 1.0f;
    if (isWriteShadow == true)
    {
        shadow = CalcShadow(i.spos);
    }
    
    resultColor.rgb *= shadow;
    
    PSOutput output;
    output.target0 = resultColor * maskIntensity + dissolveColor * colorPower * (1 - maskIntensity);
    output.target1 = float4(1 - resultColor.rgb, color.a);
    return output;
}
