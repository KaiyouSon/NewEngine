#include "HighLumi.hlsli"

V2P main(AppData i)
{
    V2P o; // �s�N�Z���V�F�[�_�[�ɓn���l
    o.svpos = mul(mat, i.pos); // ���W�ɍs�����Z
    o.uv = i.uv;
    return o;
}