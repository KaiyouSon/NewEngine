#include "BHighLumi.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex1.Sample(smp, i.uv);
    
    float grayScale = texColor.r * 0.299f + texColor.g * 0.587f + texColor.b * 0.114f;
    float extract = smoothstep(0.3f, 0.6f, grayScale);
    float power = 2.f;
    
    float4 result = float4(texColor.rgb * extract * power, 1);
    return result;
}