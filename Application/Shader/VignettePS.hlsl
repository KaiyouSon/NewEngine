#include "Vignette.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex1.Sample(smp, i.uv.xy);
    float2 newUV = i.uv.xy * 2 - 1;
    float l = length(newUV);
    float mask = 1 - smoothstep(0.6, 1.3, l);
    float invertMask = 1 - mask;
    
    float3 displayColor = texColor.rgb * mask;
    float3 vignetteColor = (1 - texColor.rgb) * float3(0, 0, 0) * invertMask;
    
    return float4(displayColor + vignetteColor, 1);
}