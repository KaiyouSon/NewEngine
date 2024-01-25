#include "LensFlare.hlsli"

Texture2D<float4> mainTex : register(t0);
SamplerState smp : register(s0);

float4 DrawCircle(float2 circleUV, float2 pixelUV, float radius, float4 color);

float4 main(V2P i) : SV_TARGET
{
    //float4 lensFlareColor = float4(1, 1, 1, 1);
    //float lensFlareIntensity = 0.5f;
    
    //float2 flarePos = float2(960.f, 540.f);
    //float flareSize = 0.1f;
    
    //float disToFlare = length(i.uv - flarePos);

    //float flareIntensity = saturate(1.0f - disToFlare / flareSize);
    //float4 flareColor = /*flareIntensity * */lensFlareIntensity * lensFlareColor;
    
    //return flareColor;
    
    //return mainTex.Sample(smp, i.uv) + flareColor;
    
    // 調整できるパラメーター
    float oddSpace = 0.1f; // 奇数時のスペース
    float evenSpace = 0.025f; // 偶数時のスペース
    float spaceAccel = 0.005f; // スペースの増加量
    
    float baseRadius = 0.025f; // ベース半径
    float radiusAccel = 0.02f; // 半径の増加量
    
    
    float asepect = 1920.0f / 1080.0f;
    float2 center = float2(960.f, 540.f);
    
    float4 col = mainTex.Sample(smp, i.uv);
    
    float2 vec = normalize(float2(1.f, 1.f));
    
    float4 result = float4(0, 0, 0, 1);
    
    float pos = float2(0.f, 0.f);
    float space = 0;
    float spaceOffset = 0;
    for (uint index = 0; index < 8; index++)
    {
        float radius = baseRadius + index * radiusAccel;
        
        result += DrawCircle(pos + vec * space, i.uv, radius, float4(1, 0, 0, 1));
        spaceOffset += (index % 2 == 1) ? index * spaceAccel : 0.0f;
        space += ((index % 2 == 0) ? evenSpace : oddSpace + spaceOffset) + index * radius / 8;
    }
    
    return /*col + */result;
}

float4 DrawCircle(float2 circleUV, float2 pixelUV, float radius, float4 color)
{
    float2 asepect = float2(1920.0f / 1080.0f, 1.0f);
    
    // 現在のピクセルと円の中心の座標との長さ
    float2 subUV = pixelUV - circleUV;
    float len = length(subUV * asepect);
    
    // 半径以下なら描画
    float3 circle = 1 - smoothstep(0, radius, len);
    
    return float4(circle, 1) * color;
}