#include "ShadowMap.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> depthTex : register(t1); // 1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(V2P i) : SV_TARGET
{
    // �e�N�X�`���[�}�b�s���O
    float4 texColor = tex.Sample(smp, i.uv);
    float d = depthTex.Sample(smp, i.uv).r;
    
    return texColor;
    //return float4(d, d * d, 0, 1);
}