#include "RadialBlur.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float4 col = 0;
    float2 symmetryUv = i.uv - 0.5;
    float distance = length(symmetryUv);
    
    float factor = strength / loopNum * distance;
    for (int j = 0; j < loopNum; j++)
    {
        float uvOffset = 1 - factor * j;
        col += tex1.Sample(smp, symmetryUv * uvOffset + 0.5f);
    }
    col /= loopNum;
    return float4(col.rgb, 1);
    
   // float4 texColor1 = GaussianBlur(tex1, smp, i.uv, 0.01f, 10);
    
   // return float4(1, 0, 0, 1);
}