#include "HighLumi.hlsli"

V2P main(AppData i)
{
    V2P o; // ピクセルシェーダーに渡す値
    o.svpos = mul(mat, i.pos); // 座標に行列を乗算
    o.uv = i.uv;
    return o;
}