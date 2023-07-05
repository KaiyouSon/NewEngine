#include "CircleGaugeSprite.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvposUv main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutputSvposUv output; // ピクセルシェーダーに渡す値
    output.svpos = mul(mat, pos); // 座標に行列を乗算
    output.uv = uv;
    return output;
}