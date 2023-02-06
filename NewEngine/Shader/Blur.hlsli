// ���m�N��
float4 Monochrome(Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    float4 monochromeColor = tex.Sample(smp, uv);
    
    float Y =
    0.299f * monochromeColor.r +
    0.587f * monochromeColor.b +
    0.114f * monochromeColor.b;
    
    monochromeColor.rgb = Y;
    return float4(monochromeColor.rgb, 1);
}

// ���σu���[
float4 AverageBlur(float2 size, float texelSpace, Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    // texelSpace���炷���߂�UV�l�����߂�
    const float offSetU = texelSpace / size.x;
    const float offSetV = texelSpace / size.y;
        
    float3 blurColor = 0;
    
    // ��e�N�Z������E�̃e�N�Z���̃J���[���T���v�����O����
    blurColor += tex.Sample(smp, uv + float2(+offSetU, 0.0f));
        
    // ��e�N�Z�����獶�̃e�N�Z���̃J���[���T���v�����O����
    blurColor += tex.Sample(smp, uv + float2(-offSetU, 0.0f));
        
    // ��e�N�Z�����牺�̃e�N�Z���̃J���[���T���v�����O����
    blurColor += tex.Sample(smp, uv + float2(0.0f, +offSetV));
        
    // ��e�N�Z�������̃e�N�Z���̃J���[���T���v�����O����
    blurColor += tex.Sample(smp, uv + float2(0.0f, -offSetV));
        
    // ��e�N�Z������E���̃e�N�Z���̃J���[���T���v�����O����
    blurColor += tex.Sample(smp, uv + float2(+offSetU, +offSetV));
        
    // ��e�N�Z������E��̃e�N�Z���̃J���[���T���v�����O����
    blurColor += tex.Sample(smp, uv + float2(+offSetU, -offSetV));
        
    // ��e�N�Z�����獶���̃e�N�Z���̃J���[���T���v�����O����
    blurColor += tex.Sample(smp, uv + float2(-offSetU, +offSetV));
        
    // ��e�N�Z�����獶��̃e�N�Z���̃J���[���T���v�����O����
    blurColor += tex.Sample(smp, uv + float2(-offSetU, -offSetV));
        
    //  ��e�N�Z���ƋߖT8�e�N�Z���̕��ςȂ̂�9�Ŋ���
    blurColor /= 9.0f;
        
    return float4(blurColor.rgb, 1);
}

// ���u���[
float4 HorizontalBlur(float width, Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    const float weight[3] = { 0.7f, 0.2f, 0.1f };

    float3 blurColor = 0;
        
    // ��e�N�Z������E��3�e�N�Z���A�d�ݕt���ŃT���v�����O
    blurColor += weight[0] * tex.Sample(smp, uv + float2(+1 / width, 0.f));
    blurColor += weight[1] * tex.Sample(smp, uv + float2(+2 / width, 0.f));
    blurColor += weight[2] * tex.Sample(smp, uv + float2(+3 / width, 0.f));
    
    // ��e�N�Z�����獶��3�e�N�Z���A�d�ݕt���ŃT���v�����O
    blurColor += weight[0] * tex.Sample(smp, uv + float2(-1 / width, 0.f));
    blurColor += weight[1] * tex.Sample(smp, uv + float2(-2 / width, 0.f));
    blurColor += weight[2] * tex.Sample(smp, uv + float2(-3 / width, 0.f));
        
    return float4(blurColor, 1);
}

// �c�u���[
float4 VerticalBlur(float height, Texture2D<float4> tex, SamplerState smp, float2 uv)
{
    const float weight[3] = { 0.7f, 0.2f, 0.1f };

    float3 blurColor = 0;
        
    // ��e�N�Z�����牺��3�e�N�Z���A�d�ݕt���ŃT���v�����O
    blurColor += weight[0] * tex.Sample(smp, uv + float2(0.f, 1 / height));
    blurColor += weight[1] * tex.Sample(smp, uv + float2(0.f, 2 / height));
    blurColor += weight[2] * tex.Sample(smp, uv + float2(0.f, +3 / height));
    
    // ��e�N�Z��������3�e�N�Z���A�d�ݕt���ŃT���v�����O
    blurColor += weight[0] * tex.Sample(smp, uv + float2(0.f, -1 / height));
    blurColor += weight[1] * tex.Sample(smp, uv + float2(0.f, -2 / height));
    blurColor += weight[2] * tex.Sample(smp, uv + float2(0.f, -3 / height));
        
    return float4(blurColor, 1);
}