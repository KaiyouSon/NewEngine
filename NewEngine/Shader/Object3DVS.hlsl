#include "Object3D.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvposPosNormalUv main(VSInputPosNormalUv vsInput)
{
    // 法線にワールド行列によるスケーリング・回転を適用		
    float4 wnormal = normalize(mul(worldMat, float4(vsInput.normal, 0)));
    float4 wpos = mul(worldMat, vsInput.pos);
    float4 vertexPos = mul(mul(viewMat, worldMat), vsInput.pos);

    // ピクセルシェーダーに渡す値
    VSOutputSvposPosNormalUv output = (VSOutputSvposPosNormalUv) 0;
    output.svpos = vertexPos;
    output.worldPos = wpos;
    output.normal = wnormal.xyz;
    output.uv = vsInput.uv;

    return output;
}