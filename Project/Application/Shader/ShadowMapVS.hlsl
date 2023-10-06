#include "ShadowMap.hlsli"

V2P main(Appdata i)
{
    V2P o; // ピクセルシェーダーに渡す値
    o.svpos = mul(mat, i.pos); // 座標に行列を乗算
    //o.spos = mul(viewProjMat, i.pos);
    o.uv = i.uv;
    return o;
}
