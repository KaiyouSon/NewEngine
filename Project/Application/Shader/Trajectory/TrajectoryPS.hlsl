#include "Trajectory.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
    float dis = abs(i.uv.x - 0.5f);
    
	// テクスチャーマッピング
    float4 texColor = tex.Sample(smp, newUV);
    texColor.a = 1 - smoothstep(0, 0.5, dis);
    
    return texColor * color;
}
