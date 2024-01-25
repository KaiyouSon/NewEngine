#include "ColliderObject.hlsli"

V2P main(Appdata i)
{
    float4 vpos = mul(wvpMat, i.pos);

    V2P output = (V2P) 0;
    output.svpos = vpos;
    return output;
}
