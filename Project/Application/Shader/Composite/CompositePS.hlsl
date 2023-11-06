#include "Composite.hlsli"

Texture2D<float4> tex1 : register(t0);
Texture2D<float4> tex2 : register(t1);
Texture2D<float4> tex3 : register(t2);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    float4 texColor1 = tex1.Sample(smp, i.uv);
    float4 texColor2 = tex2.Sample(smp, i.uv);
    float4 texColor3 = tex3.Sample(smp, i.uv);
    
    float3 color = texColor1.rgb + texColor2.rgb + texColor3.rgb;
    float4 result = float4(color, 1);
    return result;
}
