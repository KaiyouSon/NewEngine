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
}
