#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> depthTex : register(t0);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);
    return float4(texColor.rgb, 1) * color;
}