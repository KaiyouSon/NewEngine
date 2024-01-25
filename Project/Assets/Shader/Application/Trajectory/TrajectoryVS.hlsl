#include "Trajectory.hlsli"

V2P main(Appdata i)
{
    float4 vpos = mul(mul(viewProjMat, worldMat), i.pos);

    // ピクセルシェーダーに渡す値
    V2P output = (V2P) 0;
    output.svpos = vpos;
    output.uv = i.uv;

    return output;
}
