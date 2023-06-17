#include "RenderTexture.hlsli"
#include "Blur.hlsli"
#include "Lighting.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex1 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex2 : register(t1); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutputSvposUv vsOutput) : SV_TARGET
{
    // �e�N�X�`���[�}�b�s���O
    float4 texColor1 = 1 - tex1.Sample(smp, vsOutput.uv);
    float4 texColor2 = ShiftBlur(tex2, smp, vsOutput.uv, 3, 0.005);
    
    float4 result = fmod(vsOutput.uv.y, 0.1f) < 0.05f ? texColor1 : texColor2;
    
    return float4(texColor2.rgb, 1);
    
    //return Monochrome(tex, smp, vsOutput.uv);
    //return AverageBlur(float2(1920, 1080), 4, tex, smp, vsOutput.uv);
    //return HorizontalBlur(1920, tex, smp, input.uv);
    //return VerticalBlur(1080, tex, smp, input.uv);
}