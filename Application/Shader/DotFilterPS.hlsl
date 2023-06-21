#include "DotFilter.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 DotFilter(float2 uv)
{
    float2 st = uv / 1920 * 20;
    st = frac(st * float2(1920, 1080));
    float l = distance(st, float2(0.5, 0.5));

    return float4(1, 1, 1, 1) * 1 - step(0.05, l);
}

float4 main(V2P i) : SV_TARGET
{
    float4 texColor1 = tex1.Sample(smp, i.uv);
    float4 filter = DotFilter(i.uv);
    
    float4 result = texColor1 * filter;
    return float4(result.rgb, 1);
}