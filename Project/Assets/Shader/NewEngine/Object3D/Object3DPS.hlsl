#include "Object3D.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> dissolveTex : register(t1);
Texture2D<float4> shadowMapTex : register(t2);
SamplerState smp : register(s0);

// ソフトシャドウ
float CalcShadowPFC(float4 spos);

// ライティング
float4 CalcLighting(V2P i, Material material);

// ディゾルブ
float4 CalcDissolve(float2 uv, float4 color);

// 距離フォグ
float4 CalcDistanceFog(float3 wpos, float4 color);

float4 main(V2P i) : SV_TARGET
{
	// テクスチャーマッピング
    float2 newUV = (i.uv + offset) * tiling;
    float4 texColor = tex.Sample(smp, newUV);
    
    // ライティング
    Material material = { ambient, diffuse, specular };
    float4 adsColor = CalcLighting(i, material);

    float4 resultColor = (adsColor * texColor * color);
    
    // PCF
    float shadow = 1.0f;
    if (isWriteShadow == true)
    {
        shadow = CalcShadowPFC(i.spos);
    }
    resultColor.rgb *= shadow;

    // ディゾルブ    
    resultColor = CalcDissolve(newUV, resultColor);
    
    // 距離フォグ
    resultColor = CalcDistanceFog(i.wpos.xyz, resultColor);
    
    return resultColor;
}

// ソフトシャドウ
float CalcShadowPFC(float4 spos)
{
    float shadow = 1;
    
    // ライトビューでのスクリーン空間でのz値を計算する
    float z = spos.z / spos.w;
    
    // シャドウマップのUVを算出
    float2 shadowTexUV = spos.xy / spos.w;
    shadowTexUV *= float2(0.5f, -0.5f);
    shadowTexUV += 0.5f;
    
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
		    // 色取得するUV座標
            float2 offset = float2(px, py);
            float2 pickUV = shadowTexUV + offset * shiftWidth;
            
            // 画面外の色を取得しないように
            pickUV = clamp(pickUV, 0.001, 0.999);

            // シャドウ マップから深度をサンプリング
            float shadowDepth = shadowMapTex.Sample(smp, pickUV).r;
            if (shadowDepth + bias < z)
            {
                shadowFactor += 0.75f;
            }
            count++;
        }
    }

    // サンプル数で割って正規化
    shadow = 1 - shadowFactor / count;
    return shadow;
}

// ライティング
float4 CalcLighting(V2P i, Material material)
{
    // 光沢度
    const float shininess = 2.0f;
    
    float4 adsColor = float4(0, 0, 0, 1);
    float3 eyeVec = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
    
    uint index = 0;
    // 平行光源の計算
    for (index = 0; index < directionalLightSize; index++)
    {
        adsColor.rgb += CalcDirectionalLight(directionalLight[index], material, i.normal, eyeVec, shininess);
    }

    // 点光源の計算
    for (index = 0; index < pointLightSize; index++)
    {
        if (pointLight[index].isActive == false)
        {
            continue;
        }
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
    
    // スポットライトの計算
    for (index = 0; index < spotLightSize; index++)
    {
        if (spotLight[index].isActive == false)
        {
            continue;
        }
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
            
        float atten = spotLight[index].decay * ((1 - s2) * (1 - s2));
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
    
    return adsColor;
}

// ディゾルブ
float4 CalcDissolve(float2 uv, float4 color)
{
    float4 mask = dissolveTex.Sample(smp, uv);
    
    float maskIntensity = smoothstep(0.1, 0.25, (0.5f + mask.r) - dissolve);
    clip(maskIntensity - 0.01f);
    
    float4 resultColor = color * maskIntensity + dissolveColor * colorPower * (1 - maskIntensity);
    return resultColor;
}

// 距離フォグ
float4 CalcDistanceFog(float3 wpos, float4 color)
{
    if (isActiveDistanceFog == false)
    {
        return color;
    }
    
    // 距離フォグの割合
    float dis = distance(wpos.xyz * distanceRate, cameraPos);
    float distanceFogIntensity = smoothstep(fogNearFarDistance.x, fogNearFarDistance.y, dis);
    float intensity = distanceFogIntensity;
    
    if (isActiveHeightFog == true)
    {
        // 高さフォグの割合
        float height = wpos.y;
        float heightFogIntensity = smoothstep(fogNearFarHeight.x, fogNearFarHeight.y, height);
        intensity = max(distanceFogIntensity, heightFogIntensity);
    }
    
    float4 resultColor = lerp(color, distanceFogColor, intensity);
    return resultColor;
}