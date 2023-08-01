#include "RenderTexture.hlsli"
#include "Blur.hlsli"
#include "Lighting.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex1 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> depthTex : register(t1); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutputSvposUv vsOutput) : SV_TARGET
{
    // �e�N�X�`���[�}�b�s���O
    float4 texColor1 = tex1.Sample(smp, vsOutput.uv);
    float d = depthTex.Sample(smp, vsOutput.uv).r;
    
    float4 result = float4(d, d, d, 1) + float4(texColor1.rgb, 1);
    return result;
    
    return float4(d, d, d, 1);
    return float4(texColor1.rgb, 1);
    
    //return Monochrome(tex, smp, vsOutput.uv);
    //return AverageBlur(float2(1920, 1080), 4, tex, smp, vsOutput.uv);
    //return HorizontalBlur(1920, tex, smp, input.uv);
    //return VerticalBlur(1080, tex, smp, input.uv);
}