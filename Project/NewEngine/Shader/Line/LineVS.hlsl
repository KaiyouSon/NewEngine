#include "Line.hlsli"

V2P main(Appdata i)
{
    float4 vertexPos = mul(mul(viewMat, worldMat), i.pos);

    V2P o = (V2P) 0; // ピクセルシェーダーに渡す値
    o.svpos = vertexPos;
    return o;
}