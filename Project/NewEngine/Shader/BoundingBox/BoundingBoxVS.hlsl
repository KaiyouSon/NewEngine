#include "BoundingBox.hlsli"

V2P main(Appdata i)
{
    // 法線にワールド行列によるスケーリング・回転を適用		
    float4 vpos = mul(mul(viewProjMat, worldMat), i.pos);
    float4 wpos = mul(worldMat, i.pos);

    // ピクセルシェーダーに渡す値
    V2P o = (V2P) 0;
    o.svpos = vpos;
    o.uvw = i.uvw;
    o.wpos = wpos.xyz;

    return o;
}
