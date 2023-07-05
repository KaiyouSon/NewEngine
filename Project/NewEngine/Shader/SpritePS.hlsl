#include "Sprite.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

PSOutput main(VSOutputSvposUv vsInput) //: SV_TARGET
{
    //return float4(tex.Sample(smp, vsInput.uv)) * color;
    
    float4 texColor = float4(tex.Sample(smp, vsInput.uv));
    
    PSOutput output;
    output.target0 = texColor * color;
    output.target1 = texColor * color;
    return output;
}