// モノクロ
float4 Monochrome(Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    float3 monochromeColor = tex.Sample(smp, uv).rgb;
    
    float Y =
    0.299f * monochromeColor.r +
    0.587f * monochromeColor.b +
    0.114f * monochromeColor.b;
    
    monochromeColor.rgb = Y;
    return float4(monochromeColor.rgb, 1);
}

// 平均ブラー
float4 ShiftBlur(Texture2D<float4> tex, SamplerState smp, float2 uv, float shiftNum, float shiftWidth)
{
    float4 result = float4(0, 0, 0, 0);
    float num;
    
    for (float py = -shiftNum / 2; py <= shiftNum / 2; py++)
    {
        for (float px = -shiftNum / 2; px <= shiftNum / 2; px++)
        {
            result += tex.Sample(smp, uv + float2(px, py) * shiftWidth);
            num++;
        }
    }
    result.rgb = result.rgb / num;
    return float4(result.rgb, 1);
}

// 横ブラー
float4 HorizontalBlur(float width, Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    const float weight[3] = { 0.7f, 0.2f, 0.1f };

    float3 blurColor = 0;
        
    // 基準テクセルから右に3テクセル、重み付きでサンプリング
    blurColor += weight[0] * tex.Sample(smp, uv + float2(+1 / width, 0.f)).rgb;
    blurColor += weight[1] * tex.Sample(smp, uv + float2(+2 / width, 0.f)).rgb;
    blurColor += weight[2] * tex.Sample(smp, uv + float2(+3 / width, 0.f)).rgb;
    
    // 基準テクセルから左に3テクセル、重み付きでサンプリング
    blurColor += weight[0] * tex.Sample(smp, uv + float2(-1 / width, 0.f)).rgb;
    blurColor += weight[1] * tex.Sample(smp, uv + float2(-2 / width, 0.f)).rgb;
    blurColor += weight[2] * tex.Sample(smp, uv + float2(-3 / width, 0.f)).rgb;
        
    return float4(blurColor, 1);
}

// 縦ブラー
float4 VerticalBlur(float height, Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    const float weight[3] = { 0.7f, 0.2f, 0.1f };

    float3 blurColor = 0;
        
    // 基準テクセルから下に3テクセル、重み付きでサンプリング
    blurColor += weight[0] * tex.Sample(smp, uv + float2(0.f, +1 / height)).rgb;
    blurColor += weight[1] * tex.Sample(smp, uv + float2(0.f, +2 / height)).rgb;
    blurColor += weight[2] * tex.Sample(smp, uv + float2(0.f, +3 / height)).rgb;
    
    // 基準テクセルから上に3テクセル、重み付きでサンプリング
    blurColor += weight[0] * tex.Sample(smp, uv + float2(0.f, -1 / height)).rgb;
    blurColor += weight[1] * tex.Sample(smp, uv + float2(0.f, -2 / height)).rgb;
    blurColor += weight[2] * tex.Sample(smp, uv + float2(0.f, -3 / height)).rgb;
        
    return float4(blurColor, 1);
}

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

// 高輝度抽出
float4 ExtractionBrightness(float4 color, float min, float max, float power)
{
    float grayScale = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
    float extract = smoothstep(min, max, grayScale);

    return color * extract * power;
}
