#include "Emitter.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(G2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);
    
    clip(texColor.a - 0.25);
    
    float4 shineColor = i.color;
    shineColor.rgb *= i.shininess;
    
    return texColor * shineColor;
}
