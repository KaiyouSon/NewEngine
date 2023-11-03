#include "GaussianBlur.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

// ガウス関数
float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    // 描画ピクセルと色取得座標との距離
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

// ガウシアンブラー
float4 GaussianBlur(Texture2D<float4> tex, SamplerState smp, float2 uv, float sigma, float loopNum)
{
    float4 result = float4(0, 0, 0, 0);
    float totalWeight = 0;
    float max = sigma * 2;
    float min = -sigma * 2;
    float stepWidth = (max - min) / loopNum;

    [unroll]
    for (float py = min; py <= max; py += stepWidth)
    {
        [unroll]
        for (float px = min; px <= max; px += stepWidth)
        {
		    // 色取得するUV座標
            float2 offset = float2(px, py);
            float2 pickUV = uv + offset;
            
            // 画面外の色を取得しないように
            pickUV = clamp(pickUV, 0.001, 0.999);

			// ウェイト
            float weight = Gaussian(uv, pickUV, sigma);
            totalWeight += weight;

            result += tex.Sample(smp, pickUV) * weight;
        }
    }

    result.rgb = result.rgb / totalWeight;
    result.a = 1;
    return result;
}

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = GaussianBlur(tex, smp, i.uv, 0.0025f, 10);
    
    return float4(texColor.rgb, 1);
}
