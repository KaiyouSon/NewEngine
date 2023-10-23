#include "ShadowMap.hlsli"

V2P main(Appdata i)
{
    V2P o;
    o.svpos = mul(mat, i.pos);
    o.uv = i.uv;
    return o;
}
