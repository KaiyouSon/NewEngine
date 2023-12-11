#include "RadialBlur.hlsli"

Texture2D<float4> mainTex : register(t0);
Texture2D<float4> maskTex : register(t1);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    float asepect = 1920.0f / 1080.0f;
    
    // 中心座標との距離を計算
    float distance = length((i.uv - centerPos) * float2(asepect, 1.0f));

    // 円の半径
    float radius = 0.5;
    
    // 透明度をsmoothstepで計算
    float cirCol = 1 - smoothstep(0, radius, distance);
    
    //// 円内の場合は白色、それ以外は透明にする
    //if (distance <= radius)
    //{
    //    return float4(alpha, alpha, alpha, alpha); // 白色
    //}
    //else
    //{
    //    return float4(0, 0, 0, 0); // 透明
    //}
    
    float4 maskCol = maskTex.Sample(smp, i.uv) * 10;
    float mask = maskCol.r > 0 ? 1.f : 0.f;
    if (mask == 1)
    {
        return float4(0, 0, 0, 1);
    }
    
    float4 col = 0;

    float2 dir = centerPos - i.uv;
    float len = length(dir);
    
    dir = normalize(dir);
    dir *= strength * len;
    
    float totalWeight = 0;
    
    [loop]
    for (int index = 0; index < loopNum; index++)
    {
        // 重みの計算
        float weight = 1.0f - 0.08f * index;
        
        col += mainTex.Sample(smp, i.uv + dir * index) * weight;
        totalWeight += weight;

    }
    col /= totalWeight;
    

    return float4(col.rgb * cirCol, 1);
}
