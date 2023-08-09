#include "Ripple.hlsli"

// マテリアル
struct Material
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> dissolveTex : register(t1); // 0番スロットに設定されたテクスチャ
Texture2D<float4> shadowMapTex : register(t2); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
    // 円
    float len = distance(i.uv, float2(0.5f, 0.5f));
    
    // 円が外側に行くように
    float2 move = 0;
    move.x = cos(((len + offset.x) * 40.f) * 0.2);
    move.y = 0;
    
    // テクスチャーマッピング
    float4 texColor = tex.Sample(smp, i.uv + move);
    texColor.a = (1 - smoothstep(0.0f, 0.5f, len)) * 0.9f;
    
	// 光沢度
    const float shininess = 3.0f;
    float4 shainColor = color;
    shainColor.rgb *= shininess;
    
    return texColor * shainColor;
    


    // マテリアル
    Material material = { ambient, diffuse, specular };
    
    // シェーダーカラー
    float4 shaderColor = 0;
    
    float4 adsColor = float4(0, 0, 0, 1);
    if (isActiveDirLight == true)
    {
        // ライトに向かうベクトルと法線の内積
        float dotLightNormal = dot(dirLightVec, i.normal);
        
        // アンビエント
        float3 ambient = texColor.rgb * 0.6f * material.ambient.rgb;
     
        // ディフューズ
        float intensity = saturate(dot(normalize(i.normal), dirLightVec));
        float4 diffuse = intensity * dirLightColor * float4(material.diffuse.rgb, 1);
    
        // スペキュラー
        float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
        float3 reflectDir = -dirLightVec + 2 * i.normal * dot(i.normal, dirLightVec);
        float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
        adsColor.rgb = ambient + diffuse + specular * dirLightColor;
    }
    
    float4 resultColor = (adsColor * texColor * color);
    return resultColor;
}