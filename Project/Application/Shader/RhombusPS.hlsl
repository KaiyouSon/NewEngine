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
    float2 size = float2(0.3, 1);
    // ピクセルの座標を正規化して菱形の中心を原点にする
    float2 centerUV = i.uv * 2.0f - 1.0f;
    float2 newUV = i.uv * tiling;
    
    // 左右に揺らすため
    float amplitude = 0.15f; // 振幅
    float frequency = 2; // 周波数
    float offsetX = sin(offset.x * frequency + i.uv.x * frequency) * amplitude;
    
    // 菱形の方程式
    float disToCenter = abs(centerUV.x / size.y) + abs((centerUV.y + offsetX) / size.x);
    
    // uvを上に移動
    newUV.x += offset.x * 0.25f;

    // テクスチャーマッピング
    float4 texColor = tex.Sample(smp, newUV);
    
    // 菱形内のアルファ
    texColor.a = smoothstep(0.01f, 0.3f, texColor.r) * texColor.a;
    
    // 菱形の枠のアルファ
    float rate = 1 - smoothstep(0.5f, 1.0f, disToCenter);
    texColor.a *= rate;
    
    clip(texColor.a - 0.1f);
    
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