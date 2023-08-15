#include "Branch.hlsli"

V2G main(Appdata i)
{
    V2G o = (V2G) 0;
    o.pos = i.pos;
    o.scale = i.scale;
    o.rotY = i.rotY;
    return o;
}