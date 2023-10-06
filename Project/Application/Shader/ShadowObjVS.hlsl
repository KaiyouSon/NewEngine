#include "ShadowObj.hlsli"

V2P main(Appdata i)
{
    // 法線にワールド行列によるスケーリング・回転を適用		
    float4 wnormal = normalize(mul(worldMat, float4(i.normal, 0)));
    float4 wpos = mul(worldMat, i.pos);
    float4 vpos = mul(mul(viewProjMat, worldMat), i.pos);

    // ピクセルシェーダーに渡す値
    V2P output = (V2P) 0;
    output.svpos = vpos;
    output.wpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = i.uv;
    output.depth.x = length(wpos - float4(cameraPos, 1)) / 1000.f;
    output.depth.y = output.depth.x * output.depth.x;

    return output;
}
