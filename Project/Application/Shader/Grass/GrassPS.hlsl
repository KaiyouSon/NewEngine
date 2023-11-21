#include "Grass.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> shadowMapTex : register(t1);
SamplerState smp : register(s0);

float CalcShadow(float4 spos)
{
    float shadow = 1;
    
    float2 shadowTexUV = spos.xy / spos.w;
    shadowTexUV *= float2(0.5f, -0.5f);
    shadowTexUV += 0.5f;
        
    // ライトビューでのスクリーン空間でのz値を計算する
    float z = spos.z / spos.w;
        
    if (shadowTexUV.x > 0.01f && shadowTexUV.x < 0.99f &&
        shadowTexUV.y > 0.01f && shadowTexUV.y < 0.99f)
    {
        float shadowDepth = shadowMapTex.Sample(smp, shadowTexUV).r;
        if (shadowDepth + 0.0001f < z)
        {
            shadow *= 0.25f;
        }
    }
    
    return shadow;
}

float4 main(G2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);
    
    clip(texColor.a - 0.5);
    
    float shadow = 1.0f;
    shadow = CalcShadow(i.spos);

    return texColor * color * shadow;
}
