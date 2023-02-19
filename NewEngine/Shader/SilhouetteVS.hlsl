#include "Silhouette.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvposNormalUv main(VSInputPosNormalUv vsInput)
{
	// 法線にワールド行列によるスケーリング・回転を適用		
    float4 wnormal = normalize(mul(worldMat, float4(vsInput.normal, 0)));

    matrix mat = mul(viewMat, worldMat);

    VSOutputSvposNormalUv output; // ピクセルシェーダーに渡す値
    output.svpos = mul(mat, vsInput.pos);
    output.normal = wnormal.xyz;
    output.uv = vsInput.uv;

    return output;
}