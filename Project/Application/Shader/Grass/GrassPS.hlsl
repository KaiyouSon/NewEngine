#include "Grass.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> shadowMapTex : register(t1);
SamplerState smp : register(s0);

// POMの計算
float3 CalcParallaxMapping(G2P i);

// ライティングの計算
float4 CalcLighting(G2P i);

// 陰の計算
float CalcShadow(float4 spos);

float4 main(G2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);
    clip(texColor.a - 0.75f);
    float4 resultColor = texColor;
    
    // POM
    float4 pomColor = float4(CalcParallaxMapping(i).rgb, 1);
    //return texColor * 10;
    //return pomColor;
    
    resultColor *= pomColor * color;
    
    return resultColor;
    
    // ライティング
    float4 adsColor = CalcLighting(i);
    resultColor *= adsColor;
    
    // シャドウ
    float shadow = 1.0f;
    shadow = CalcShadow(i.spos);
    resultColor.rgb *= shadow;
    
    return resultColor;
}

float3 CalcParallaxMapping(G2P i)
{
    float3x3 tbnMat = float3x3(i.tangent, i.binormal, float3(0, 0, 1));
    
    // レイ(ワールド座標系)
    float3 rayDir = normalize(cameraPos - i.wpos.xyz);
    rayDir = mul(tbnMat, rayDir);
    
    float height = tex.Sample(smp, i.uv).r;
    float2 offsetUV = float2(-rayDir.x, rayDir.y) * (height * heightScale);
    float2 shiftUV = saturate(i.uv + offsetUV);
    
    return tex.Sample(smp, shiftUV).rgb;
    
    
    
    //// 交点を計算する
    //float3 planeNormal = float3(0, 0, -1); // 平面の法線ベクトル
    //float3 planeOrigan = i.opos; // 平面上の原点
    
    //// カメラの座標から長さtをかけて交点を求める
    //float t = dot(planeOrigan - cameraPos, planeNormal) / dot(rayDir, planeNormal);
    //float3 crossPoint = cameraPos + t * rayDir;
    
    //// 交点から中心点に向かうベクトル
    //float3 toCenter = crossPoint - planeOrigan;
    
    //// 平面のxy軸（仮）
    //float3 xAxis = float3(1, 0, 0);
    //float3 yAxis = float3(0, 1, 0);

    //// 交点のuvを算出
    //float2 crossUV = toCenter.xy / float2(20.0f, -20.0f) + float2(0.5f, 0.0f);
    ////crossUV = saturate(crossUV);
    
    //// 交点uvでハイトマップをサンプリングし高さを取得する
    //float height = tex.Sample(smp, crossUV).r;
    //float2 offset = rayDir.xy * (height.xx * heightScale.xx);
    //float2 shiftedUV = i.uv + offset;
    //shiftedUV = saturate(shiftedUV);
    
    //float3 pomColor = tex.Sample(smp, shiftedUV).rgb;
    //return pomColor;
}

float4 CalcLighting(G2P i)
{
    Material material;
    material.ambient = ambient;
    material.diffuse = diffuse;
    material.specular = specular;
    
    // 光沢度
    const float shininess = 2.0f;
    
    float4 adsColor = 0;
    
    uint index = 0;
    // 平行光源の計算
    for (index = 0; index < directionalLightSize; index++)
    {
        if (directionalLight[index].isActive == true)
        {
            // ライトに向かうベクトルと法線の内積
            float dotLightNormal = dot(directionalLight[index].vec, i.vnormal);
        
            // アンビエント
            float3 ambient = material.ambient.rgb + float3(0.2f, 0.2f, 0.2f);
     
            // ディフューズ
            float3 diffuse = dotLightNormal * material.diffuse.rgb;
    
            // スペキュラー
            float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
            float3 halfVector = normalize(-directionalLight[index].vec + eyeDir);
            float nDoth = dot(normalize(i.vnormal), halfVector);
            float3 specular = pow(saturate(nDoth), shininess) * material.specular.rgb;
    
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
            float3 dotLightNormal = dot(lightVec, i.vnormal);
            
            // ディフューズ
            float3 diffuse = saturate(dotLightNormal * material.diffuse.rgb);
    
            // スペキュラー
            float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
            float3 reflectDir = normalize(-lightVec + 2 * dotLightNormal * i.vnormal);
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
            float3 dotLightNormal = dot(lightVec, i.vnormal);
            
            // ディフューズ
            float3 diffuse = saturate(dotLightNormal * material.diffuse.rgb);
    
            // スペキュラー
            float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
            float3 reflectDir = normalize(-lightVec + 2 * dotLightNormal * i.vnormal);
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
    
    // ライトビューでのスクリーン空間でのz値を計算する
    float z = spos.z / spos.w;
    
    // シャドウマップのUVを算出
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
		    // 色取得するUV座標
            float2 offset = float2(px, py);
            float2 pickUV = shadowTexUV + offset * shiftWidth;
            
            // 画面外の色を取得しないように
            pickUV = clamp(pickUV, 0.001, 0.999);

            // シャドウ マップから深度をサンプリング
            float shadowDepth = shadowMapTex.Sample(smp, pickUV).r;
            if (shadowDepth + 0.0005f < z)
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

