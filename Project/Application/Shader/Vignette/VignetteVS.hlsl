#include "Vignette.hlsli"

V2P main(AppData i)
{
    V2P o;
    o.svpos = mul(mat, i.pos);
    o.uv = i.uv;
    return o;
}
