#include "RadialBlur.hlsli"

Texture2D<float4> mainTex : register(t0);
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
