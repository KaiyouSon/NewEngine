#include "WorldVolumetricFog.hlsli"

V2P main(Appdata i)
{
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
    float4 wpos = mul(worldMat, i.pos);
    float4 vpos = mul(viewProjMat, wpos);

    // �s�N�Z���V�F�[�_�[�ɓn���l
    V2P o = (V2P) 0;
    o.svpos = vpos;
    o.uvw = i.uvw;
    o.wpos = wpos.xyz;

    return o;
}
