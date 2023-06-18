#include "GaussainBlur.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

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
    float stepWidth = sigma / loopNum;

    for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
    {
				[loop]
        for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
        {
					// 色取得するUV座標
            float2 pickUV = uv + float2(px, py);

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
    float4 texColor1 = tex1.Sample(smp, i.uv);
    float4 texColor2 = GaussianBlur(tex1, smp, i.uv, 0.01f, 10);
    
    float4 result = texColor1 + texColor2;
    
    return float4(result.rgb, 1);
}