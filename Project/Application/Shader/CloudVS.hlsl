#include "Cloud.hlsli"

V2P main(Appdata i)
{
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
    float4 wpos = mul(worldMat, i.pos);
    float4 vertexPos = mul(mul(viewMat, worldMat), i.pos);

    // �s�N�Z���V�F�[�_�[�ɓn���l
    V2P o = (V2P) 0;
    o.svpos = vertexPos;
    o.worldPos = wpos;
    o.uv = i.uv;

    return o;
}