#include "Cloud.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float radius = 0.4;
    float r = distance(i.uv, float2(0.5, 0.5));
    
    float s = smoothstep(0.05, 0.5, r);
    
    clip(1 - s);
    
    float2 newUV = (i.uv + offset) * tiling;
    // テクスチャーマッピング
    float4 texColor = tex.Sample(smp, newUV);
     
    float c = texColor.r;
    float rate = smoothstep(0.2f, 0.8f, c);

    float a = rate * (1 - s);
    
    float4 result = float4(c, c, c, a);
    return result * color;

}