#include "RespawnTransition.hlsli"

V2P main(Appdata i)
{
    V2P o; // �s�N�Z���V�F�[�_�[�ɓn���l
    o.svpos = mul(mat, i.pos); // ���W�ɍs�����Z
    o.uv = i.uv;
    return o;
}