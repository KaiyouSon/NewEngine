#include "RenderTexture.hlsli"
#include "Blur.hlsli"
#include "Lighting.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutputSvposUv vsOutput) : SV_TARGET
{
    // �e�N�X�`���[�}�b�s���O
    float4 texColor = tex.Sample(smp, vsOutput.uv);
    return texColor;
    
    //PSOutput output;
    
    //output.target0 = texColor;
    //output.target1 = float4(1 - texColor.rgb, 1);
    //return output;
    
    //return Monochrome(tex, smp, vsOutput.uv);
    //return AverageBlur(float2(1920, 1080), 4, tex, smp, vsOutput.uv);
    //return HorizontalBlur(1920, tex, smp, input.uv);
    //return VerticalBlur(1080, tex, smp, input.uv);
}