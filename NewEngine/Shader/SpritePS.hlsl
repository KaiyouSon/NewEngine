#include "Sprite.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

PSOutput main(VSOutputSvposUv vsInput) //: SV_TARGET
{
    //return float4(tex.Sample(smp, vsInput.uv)) * color;
    
    PSOutput output;
    output.target0 = float4(tex.Sample(smp, vsInput.uv)) * color;
    output.target1 = float4(tex.Sample(smp, vsInput.uv)) * color;
    return output;
}