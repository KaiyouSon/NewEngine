#include "ShadowMap.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> depthTex : register(t1); // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    // テクスチャーマッピング
    float4 texColor = tex.Sample(smp, i.uv);
    float d = depthTex.Sample(smp, i.uv).r;
    
    return float4(d, 0, 0, 1);
    
    //return texColor;
    return float4(d, d * d, 0, 1);
}
