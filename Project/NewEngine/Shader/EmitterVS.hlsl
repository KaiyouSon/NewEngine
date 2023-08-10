#include "Emitter.hlsli"

V2G main(Appdata i)
{
    V2G o = (V2G) 0;
    o.pos = i.pos;
    o.scale = i.scale;
    o.rot = i.rot;
    o.shininess = i.shininess;
    o.color = i.color;
    return o;
}