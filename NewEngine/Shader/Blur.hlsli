// ���m�N��
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

// ���σu���[
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

// ���u���[
float4 HorizontalBlur(float width, Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    const float weight[3] = { 0.7f, 0.2f, 0.1f };

    float3 blurColor = 0;
        
    // ��e�N�Z������E��3�e�N�Z���A�d�ݕt���ŃT���v�����O
    blurColor += weight[0] * tex.Sample(smp, uv + float2(+1 / width, 0.f)).rgb;
    blurColor += weight[1] * tex.Sample(smp, uv + float2(+2 / width, 0.f)).rgb;
    blurColor += weight[2] * tex.Sample(smp, uv + float2(+3 / width, 0.f)).rgb;
    
    // ��e�N�Z�����獶��3�e�N�Z���A�d�ݕt���ŃT���v�����O
    blurColor += weight[0] * tex.Sample(smp, uv + float2(-1 / width, 0.f)).rgb;
    blurColor += weight[1] * tex.Sample(smp, uv + float2(-2 / width, 0.f)).rgb;
    blurColor += weight[2] * tex.Sample(smp, uv + float2(-3 / width, 0.f)).rgb;
        
    return float4(blurColor, 1);
}

// �c�u���[
float4 VerticalBlur(float height, Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    const float weight[3] = { 0.7f, 0.2f, 0.1f };

    float3 blurColor = 0;
        
    // ��e�N�Z�����牺��3�e�N�Z���A�d�ݕt���ŃT���v�����O
    blurColor += weight[0] * tex.Sample(smp, uv + float2(0.f, +1 / height)).rgb;
    blurColor += weight[1] * tex.Sample(smp, uv + float2(0.f, +2 / height)).rgb;
    blurColor += weight[2] * tex.Sample(smp, uv + float2(0.f, +3 / height)).rgb;
    
    // ��e�N�Z��������3�e�N�Z���A�d�ݕt���ŃT���v�����O
    blurColor += weight[0] * tex.Sample(smp, uv + float2(0.f, -1 / height)).rgb;
    blurColor += weight[1] * tex.Sample(smp, uv + float2(0.f, -2 / height)).rgb;
    blurColor += weight[2] * tex.Sample(smp, uv + float2(0.f, -3 / height)).rgb;
        
    return float4(blurColor, 1);
}

