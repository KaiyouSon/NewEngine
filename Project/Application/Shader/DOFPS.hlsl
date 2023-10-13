#include "DOF.hlsli"

Texture2D<float4> tex1 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> depthTex : register(t1); // 0番スロットに設定されたテクスチャ
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

    float curDepth = depthTex.Sample(smp, uv).r;
    
    for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
    {
        for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
        {
		    // 色取得するUV座標
            float2 pickUV = uv + float2(px, py);
            
            // 画面外の色を取得しないように
            pickUV = clamp(pickUV, 0.001, 0.999);
            
            float pickDepth = depthTex.Sample(smp, pickUV).r;
            float diff = abs(curDepth - pickDepth);

			// ウェイト
            // モデルの深度差が一定値以上の場合は0にする
            float weight = (diff > 0.0001) ? 0.0 : Gaussian(uv, pickUV, sigma) * (1 - diff);
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
    
    float depth = depthTex.Sample(smp, i.uv).r;
    float inFocus = 1 - smoothstep(focusDepth, middleDepth, depth);
    float outFocus = smoothstep(middleDepth, 1, depth);
    float middleFocus = 1 - inFocus - outFocus;
   
    float4 inFocusColor = (isRGB == true) ? float4(1, 0, 0, 1) : tex1.Sample(smp, i.uv);
    float4 middleFocusColor = (isRGB == true) ? float4(0, 1, 0, 1) : GaussianBlur(tex1, smp, i.uv, 0.005f, 5);
    float4 outFocusColor = (isRGB == true) ? float4(0, 0, 1, 1) : GaussianBlur(tex1, smp, i.uv, 0.05f, 5);
    
    float4 result =
        inFocus * inFocusColor +
        middleFocus * middleFocusColor +
        outFocus * outFocusColor;
    
    return float4(result.rgb, 1);
}
