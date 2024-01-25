#include "Vignette.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> maskTex : register(t1);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    //return float4(1, 0, 0, 1);
    
    float4 texColor = tex.Sample(smp, i.uv);
    float2 newUV = i.uv * 2 - 1;
    float l = length(newUV);
    float mask = 1 - smoothstep(vignetteRange.x, vignetteRange.y, l);
    float invertMask = 1 - mask;
    
    float4 maskTexColor = maskTex.Sample(smp, i.uv);
    
    if (maskTexColor.r > 0 || maskTexColor.g > 0 || maskTexColor.b > 0)
    {
        return float4(texColor.rgb, 1);
        
    }
    float3 displayCol = texColor.rgb * mask;
    float3 vignetteCol = (1 - texColor.rgb) * vignetteColor.rgb * invertMask;
    return float4(displayCol + vignetteCol, 1);
}
