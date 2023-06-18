#include "HighLumi.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex1.Sample(smp, i.uv);
    
    float grayScale = texColor.r * 0.299 + texColor.g * 0.587 + texColor.b * 0.114;
    float extract = smoothstep(0.6, 0.9, grayScale);
    float power = 1.f;
    
    float4 result = float4(texColor.rgb * extract * power, 1);
    return result;
}