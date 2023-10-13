#include "Ripple.hlsli"

V2P main(Appdata i)
{
    // 法線にワールド行列によるスケーリング・回転を適用		
    float4 wnormal = normalize(mul(worldMat, float4(i.normal, 0)));
    float4 wpos = mul(worldMat, i.pos);
    float4 spos = mul(mul(lightViewMat, worldMat), i.pos);
    float4 vpos = mul(mul(viewProjMat, worldMat), i.pos);

    // ピクセルシェーダーに渡す値
    V2P output = (V2P) 0;
    output.svpos = vpos;
    output.wpos = wpos;
    output.spos = spos;
    output.normal = wnormal.xyz;
    output.uv = i.uv;

    return output;
}
