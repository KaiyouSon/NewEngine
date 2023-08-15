#include "Branch.hlsli"

V2P main(Appdata i)
{
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
    float4 wnormal = normalize(mul(worldMat, float4(i.normal, 0)));
    float4 vpos = mul(mul(viewProjMat, worldMat), i.pos);
    float4 wpos = mul(worldMat, i.pos);
    //spos.z = length(wpos - float4(lightCameraPos, 1)) / 1000.f;

    // �s�N�Z���V�F�[�_�[�ɓn���l
    V2P o = (V2P) 0;
    o.svpos = vpos;
    o.wpos = wpos;
    o.normal = wnormal.xyz;
    o.uv = i.uv;

    return o;
}