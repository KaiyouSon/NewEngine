#include "Cloud.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    float radius = 0.5;
    float r = distance(i.uv, float2(0.5, 0.5));
    
    float dis = 1 - smoothstep(0.05, radius, r);
    
    // テクスチャーマッピング
    float2 newUV = (i.uv + offset) * tiling;
    float4 texColor = tex.Sample(smp, newUV);
     
    float c = texColor.r;
    float rate = smoothstep(0.2f, 0.8f, c);

    float a = rate * dis;
    
    float4 result = float4(c, c, c, a);
    return result * color;
}