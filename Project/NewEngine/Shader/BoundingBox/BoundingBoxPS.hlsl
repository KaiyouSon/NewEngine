#include "BoundingBox.hlsli"

Texture3D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uvw);
    
    
    //return float4(texColor.r, 0, 0, 1);
    //return float4(i.uvw.z, 0, 0, 1);
    return texColor;
}