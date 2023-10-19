#include "Sprite.hlsli"

V2P main(Appdata i)
{
    V2P o = (V2P) 0;
    o.svpos = mul(mat, i.pos);
    o.uv = i.uv;
    return o;
}
