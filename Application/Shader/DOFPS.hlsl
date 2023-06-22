#include "DOF.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex2 : register(t1); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float4 texColor1 = tex1.Sample(smp, i.uv);
    float4 texColor2 = tex2.Sample(smp, i.uv);
    
    float4 result = float4(texColor1.rgb, 1);
    //return result;
    
    float depth = tex2.Sample(smp, i.uv).r;
    float4 inFocusColor = float4(1, 0, 0, 1);
    float4 outFocusColor = float4(0, 0, 1, 1);
    float focus = 1 - smoothstep(0, focusWidth, abs(depth - focusDepth));

    result = (1 - focus) * inFocusColor + focus * outFocusColor;
    
    return float4(result.rgb, 1);
    //return float4(depth, depth, depth, 1);
}