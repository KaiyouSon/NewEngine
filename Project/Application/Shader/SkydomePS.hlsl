#include "Skydome.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex1.Sample(smp, i.uv.xy);
    float2 newUV = i.uv.xy * 2 - 1;
    float l = length(newUV);
    float mask = 1 - smoothstep(vignetteRange.x, vignetteRange.y, l);
    float invertMask = 1 - mask;
    
    float3 displayCol = texColor.rgb * mask;
    float3 vignetteCol = (1 - texColor.rgb) * vignetteColor.rgb * invertMask;
    
    return float4(displayCol + vignetteCol, 1);
}
