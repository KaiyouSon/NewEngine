#include "VolumetricFog.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(V2P i) : SV_TARGET
{
    // �e�N�X�`���[�}�b�s���O
    float4 texColor = tex.Sample(smp, i.uv);
    
    return csOut[1921].color;
    return /*float4(texColor.rgb, 1) **/i.color;
}