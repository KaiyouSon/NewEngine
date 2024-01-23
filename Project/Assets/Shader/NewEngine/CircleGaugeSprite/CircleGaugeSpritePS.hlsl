#include "CircleGaugeSprite.hlsli"
#include "../../Util.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    const float PI = 3.14159f;
    
    float2 center = 0.5f;

    float2 v1 = { cos(startRadian), sin(startRadian) }; // 始点のベクトル
    float2 v2 = { cos(endRadian), sin(endRadian) }; // 終点のベクトル
    float2 v3 = i.uv - center; // 各ピクセルのベクトル
    
    // 正規化
    v1 = normalize(v1);
    v2 = normalize(v2);
    v3 = normalize(v3);
    
    float cross1 = Float2Cross(v1, v3); // 外積（左右判定用）
    if (endRadian - startRadian <= radians(180))
    {
        float dot1 = dot(v1, v2); // 始点のベクトルと終点のベクトルの内積
        float dot2 = dot(v1, v3); // 始点のベクトルと各ピクセルのベクトルの内積
        
        // ピクセルのベクトルの角度が、終点のベクトルの角度より小さい時
        if (dot1 <= dot2)
        {
            // ピクセルの座標が始点のベクトルの右側にある時
            if (cross1 >= 0.001)
            {
                clip(-1);
            }
        }
    }
    else // 180度超えた時の処理
    {
        float dot1 = dot(-v1, v2); // [180度を始点としたのベクトル]と[終点のベクトル]の内積
        float dot2 = dot(-v1, v3); // [180度を始点としたのベクトル]と[各ピクセルのベクトル]の内積
        
        float cross2 = Float2Cross(-v1, v3); // 外積（左右判定用）
        
        // ピクセルの座標が始点のベクトルの右側にある時
        if (dot1 <= dot2)
        {
            // [ピクセルの座標]が[180度を始点としたのベクトル]の右側にある時
            if (cross2 >= 0)
            {
                clip(-1);
            }
        }
        
        if (cross1 >= 0 && cross2 <= 0)
        {
            clip(-1);
        }
    }
    
    return float4(tex.Sample(smp, i.uv)) * color;
}