#include "LensFlare.hlsli"

Texture2D<float4> mainTex : register(t0);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    float asepect = 1920.0f / 1080.0f;
    float2 center = float2(960.f, 540.f);
    
    float4 col = mainTex.Sample(smp, i.uv);
    
    float2 vec = normalize(float2(-1.f, 1.f));
    
    float4 result = float4(0, 0, 0, 1);
    for (uint index = 0; index < 5; index++)
    {
        float pos = float2(0.5f, 0.5f);
        
        // 中心座標との距離を計算
        float distance = length((i.uv - (pos + vec * ((index + 1) * 0.05f/* + index * 0.025f*/))) * float2(asepect, 1.0f));
        
        // 円の半径
        float radius = 0.025f + index * 0.01f;
    
        // 透明度をsmoothstepで計算
        float3 cirCol = 1 - smoothstep(0, radius, distance);
    
        result += float4(cirCol.rgb, 1) * 2;
    }
    return col + result;
    
    //// レンズフレアの計算
    //float2 lensFlareBaseUV = float2(1.f, 1.f) - i.uv;
    
    //float2 center = float2(960.f, 540.f);
    //float2 toCenter = (center - lensFlareBaseUV) * 0.5f;
    
    //for (uint index = 0; index < 8; index++)
    //{
    //    const float2 lensFlareUV = lensFlareBaseUV + toCenter * (1.f + index);

    //    const float r = length(center - lensFlareUV);
    //    //const float fadeoutR = (r - 2);
    //    //const float weight =
    //    //    (r / length(center)) // 光源が重なったときに極端に明るくさせない重み
    //    //    * (1.0f - smoothstep(0.0f, 1, r - 2)) // 外周をフェードアウトさせる重み
    //    //;
    //    // 重みの計算
    //    float weight = 1.0f - 0.08f * index;
    //    result.rgb += col.rgb * weight;
    //}
    
    //return float4(result.rgb, 1);
    
    //float asepect = 1920.0f / 1080.0f;
    
    //// 中心座標との距離を計算
    //float distance = length((i.uv - centerPos) * float2(asepect, 1.0f));

    //// 円の半径
    //float radius = 0.5;
    
    //// 透明度をsmoothstepで計算
    //float cirCol = 1 - smoothstep(0, radius, distance);
    

    //float2 dir = centerPos - i.uv;
    //float len = length(dir);
    
    //dir = normalize(dir);
    //dir *= strength * len;
    
    //float totalWeight = 0;
    
    //[loop]
    //for (int index = 0; index < loopNum; index++)
    //{
    //    // 重みの計算
    //    float weight = 1.0f - 0.08f * index;
        
    //    col += mainTex.Sample(smp, i.uv + dir * index) * weight;
    //    totalWeight += weight;

    //}
    //col /= totalWeight;
    

    //return float4(col.rgb * cirCol, 1);
}
