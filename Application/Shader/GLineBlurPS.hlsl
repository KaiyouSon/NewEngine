#include "GLineBlur.hlsli"

Texture2D<float4> tex1 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

// �K�E�X�֐�
float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    // �`��s�N�Z���ƐF�擾���W�Ƃ̋���
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

// ���C���u���[
float4 LineBlur(Texture2D<float4> tex, SamplerState smp, float2 uv, float sigma, float loopNum)
{
    float totalWeight = 0;
    float4 color = float4(0, 0, 0, 0);
    float stepWidth = sigma / loopNum;
    float angleRad = angle * 3.14159 / 180;

    for (float j = -sigma; j <= sigma; j += stepWidth)
    {
        float x = cos(angleRad) * j;
        float y = sin(angleRad) * j;
        float2 pickUV = uv + float2(x, y);
        
        // ��ʊO�̐F���擾���Ȃ��悤��
        pickUV = clamp(pickUV, 0.001, 0.999);

        float weight = Gaussian(uv, pickUV, sigma);
        totalWeight += weight;

        color += tex.Sample(smp, pickUV) * weight;
    }

    color = color / totalWeight;
    color.a = 1;
    return color;
}

float4 main(V2P i) : SV_TARGET
{
    float4 lineBlur = LineBlur(tex1, smp, i.uv, 0.05f, 30);
    float4 result = lineBlur;
    
    return float4(result.rgb, 1);
}