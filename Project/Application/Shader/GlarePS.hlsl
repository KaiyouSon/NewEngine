#include "Glare.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex2 : register(t1); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float4 texColor1 = tex1.Sample(smp, i.uv);
    float4 texColor2 = tex2.Sample(smp, i.uv);
    
    float4 result = texColor1 + texColor2;
    return float4(result.rgb, 1);
}
