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
float4 AverageBlur(float2 size, float texelSpace, Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    // texelSpaceずらすためのUV値を求める
    const float offSetU = texelSpace / size.x;
    const float offSetV = texelSpace / size.y;
        
    float3 blurColor = 0;
    
    // 基準テクセルから右のテクセルのカラーをサンプリングする
    blurColor += tex.Sample(smp, uv + float2(+offSetU, 0.0f)).rgb;
        
    // 基準テクセルから左のテクセルのカラーをサンプリングする
    blurColor += tex.Sample(smp, uv + float2(-offSetU, 0.0f)).rgb;
        
    // 基準テクセルから下のテクセルのカラーをサンプリングする
    blurColor += tex.Sample(smp, uv + float2(0.0f, +offSetV)).rgb;
        
    // 基準テクセルから上のテクセルのカラーをサンプリングする
    blurColor += tex.Sample(smp, uv + float2(0.0f, -offSetV)).rgb;
        
    // 基準テクセルから右下のテクセルのカラーをサンプリングする
    blurColor += tex.Sample(smp, uv + float2(+offSetU, +offSetV)).rgb;
        
    // 基準テクセルから右上のテクセルのカラーをサンプリングする
    blurColor += tex.Sample(smp, uv + float2(+offSetU, -offSetV)).rgb;
        
    // 基準テクセルから左下のテクセルのカラーをサンプリングする
    blurColor += tex.Sample(smp, uv + float2(-offSetU, +offSetV)).rgb;
        
    // 基準テクセルから左上のテクセルのカラーをサンプリングする
    blurColor += tex.Sample(smp, uv + float2(-offSetU, -offSetV)).rgb;
        
    //  基準テクセルと近傍8テクセルの平均なので9で割る
    blurColor /= 9.0f;
        
    return float4(blurColor.rgb, 1);
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