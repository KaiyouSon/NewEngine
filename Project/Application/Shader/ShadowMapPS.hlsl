#include "ShadowMap.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> depthTex : register(t1); // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
    float2 shadowTexUV = i.spos.xy / i.spos.w;
    shadowTexUV *= float2(0.5f, -0.5f);
    shadowTexUV += 0.5f;
    
    float depth = depthTex.Sample(smp, shadowTexUV).r;
        
    float dis;
    float lightZ = depth * 2.0 - 1.0; // 深度を[0, 1]から[-1, 1]に変換
    float4 lightPosHomog = float4(i.spos.xy, lightZ, 1.0);
    float4 lightPosView = mul(viewProjMat, lightPosHomog);
    dis = length(lightPosView.xyz);
    
    float dep = min(1.0f, dis / 1000.f);
    //return float4(dep, dep * dep, 0.0f, 1.0f);
    
    
    // テクスチャーマッピング
    float4 texColor = tex.Sample(smp, i.uv);
    float d = depthTex.Sample(smp, i.uv).r;
    
    return texColor;
    //return float4(d, d, d, 1);
}