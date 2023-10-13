#include "ShadowObj.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
	// テクスチャーマッピング
    float4 texColor = tex.Sample(smp, i.uv);
    
    clip(texColor.a - 0.5f);
    
    //return float4(i.depth, 0, 1);
    
    return texColor;
}
