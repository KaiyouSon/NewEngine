#include "ShadowMap.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> depthTex : register(t1);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    // テクスチャーマッピング
    //float4 texColor = tex.Sample(smp, i.uv);
    //return float4(texColor.rg, 0, 1);
    
    float d = depthTex.Sample(smp, i.uv).r;
    return float4(d, 0, 0, 1);
}
