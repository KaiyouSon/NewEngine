#include "DOF.hlsli"

Texture2D<float4> tex1 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> depthTex : register(t1); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

// �K�E�X�֐�
float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    // �`��s�N�Z���ƐF�擾���W�Ƃ̋���
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

// �K�E�V�A���u���[
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
		    // �F�擾����UV���W
            float2 pickUV = uv + float2(px, py);
            
            // ��ʊO�̐F���擾���Ȃ��悤��
            pickUV = clamp(pickUV, 0.001, 0.999);
            
            float pickDepth = depthTex.Sample(smp, pickUV).r;
            float diff = abs(curDepth - pickDepth);

			// �E�F�C�g
            // ���f���̐[�x�������l�ȏ�̏ꍇ��0�ɂ���
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
    float middleDepth = depth + (1 - depth) / 2;
    float inFocus = 1 - smoothstep(focusDepth, middleDepth, depth);
    float outFocus = smoothstep(middleDepth, 1, depth);
    float middleFocus = 1 - inFocus - outFocus;
    
    //float4 inFocusColor = float4(1, 0, 0, 1);
    //float4 middleFocusColor = float4(0, 1, 0, 1);
    //float4 outFocusColor = float4(0, 0, 1, 1);
    
    float4 inFocusColor = (isRGB == true) ? float4(1, 0, 0, 1) : tex1.Sample(smp, i.uv);
    float4 middleFocusColor = (isRGB == true) ? float4(0, 1, 0, 1) : GaussianBlur(tex1, smp, i.uv, 0.005f, 5);
    float4 outFocusColor = (isRGB == true) ? float4(0, 0, 1, 1) : GaussianBlur(tex1, smp, i.uv, 0.05f, 5);
    
    float4 result =
        inFocus * inFocusColor +
        middleFocus * middleFocusColor +
        outFocus * outFocusColor;
    
    return float4(result.rgb, 1);
}