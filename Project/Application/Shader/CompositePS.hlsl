#include "Composite.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex2 : register(t1); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float4 texColor1 = tex1.Sample(smp, i.uv);
    float4 texColor2 = tex2.Sample(smp, i.uv);
    //return float4(texColor2.rgb, 1);
    
    float4 result = float4(texColor1.rgb + texColor2.rgb, 1);
    return result;
}
