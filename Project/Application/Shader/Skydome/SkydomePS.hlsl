#include "Skydome.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> dissolveTex : register(t1); // 0番スロットに設定されたテクスチャ
Texture2D<float4> shadowMapTex : register(t2); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
	// テクスチャーマッピング
    float4 texColor = tex.Sample(smp, newUV);

	// 光沢度
    const float shininess = 4.0f;

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
        float3 ambient = /*texColor.rgb **/material.ambient.rgb;
     
        // ディフューズ
        float intensity = saturate(dot(normalize(i.normal), dirLightVec));
        float3 diffuse = intensity * dirLightColor.rgb * material.diffuse.rgb;
    
        // スペキュラー
        float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
        float3 reflectDir = -dirLightVec + 2 * i.normal * dot(i.normal, dirLightVec);
        float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
        adsColor.rgb = ambient + diffuse + specular * dirLightColor.rgb;
    }
    
    float4 resultColor = (adsColor * texColor * color);
    
    return resultColor;
}
