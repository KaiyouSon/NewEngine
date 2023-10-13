#include "BoundingBox.hlsli"

Texture3D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uvw);
    
    
    //return float4(texColor.r, 0, 0, 1);
    //return float4(i.uvw.z, 0, 0, 1);
    return texColor;
}