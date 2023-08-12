#include "RespawnTransition.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
    // テクスチャーマッピング
    float4 texColor = tex.Sample(smp, newUV);
    
    float rate = 1 - smoothstep(0.0f, max, texColor.r);
    
    texColor.a = rate;
    
    return texColor * color;
    return float4(1, 0, 0, 1);
}