#include "VolumetricFog.hlsli"

V2P main(Appdata i, uint SV_VertexID : SV_VertexID)
{
    V2P o;
    o.svpos = mul(mat, i.pos); // ���W�ɍs�����Z
    o.uv = i.uv;
    o.color = csOut[SV_VertexID].color;

    return o;
}