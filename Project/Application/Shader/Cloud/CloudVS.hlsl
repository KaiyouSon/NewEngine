#include "Cloud.hlsli"

V2P main(Appdata i)
{
    // 法線にワールド行列によるスケーリング・回転を適用		
    float4 wpos = mul(worldMat, i.pos);
    float4 vertexPos = mul(viewMat, wpos);

    V2P o = (V2P) 0;
    o.svpos = vertexPos;
    o.worldPos = wpos;
    o.uv = i.uv;

    return o;
}
