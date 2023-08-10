#include "Emitter.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(G2P i) : SV_TARGET
{
    //return float4(i.pos.rgb, 1);
    
    float4 texColor = tex.Sample(smp, i.uv);
    
    clip(texColor.a - 0.5);
    
    float4 shineColor = i.color;
    shineColor.rgb *= i.shininess;
    
    return texColor * shineColor;
}