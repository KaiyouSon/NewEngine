#include "Skydome.hlsli"

V2P main(Appdata i)
{
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
    float4 wnormal = normalize(mul(worldMat, float4(i.normal, 0)));
    float4 wpos = mul(worldMat, i.pos);
    float4 vpos = mul(viewProjMat, wpos);
    float4 spos = mul(lightViewProjMat, wpos);

    // �s�N�Z���V�F�[�_�[�ɓn���l
    V2P output = (V2P) 0;
    output.svpos = vpos;
    output.wpos = wpos;
    output.spos = spos;
    output.normal = wnormal.xyz;
    output.uv = i.uv;

    return output;
}