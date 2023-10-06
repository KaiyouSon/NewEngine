#include "Sprite.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvposUv main(VSInputPosUv vsInput)
{
    VSOutputSvposUv output; // ピクセルシェーダーに渡す値
    output.svpos = mul(mat, vsInput.pos); // 座標に行列を乗算
    output.uv = vsInput.uv;
    return output;
}
