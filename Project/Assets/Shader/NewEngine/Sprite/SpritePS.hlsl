#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);
    return texColor * color;
}
