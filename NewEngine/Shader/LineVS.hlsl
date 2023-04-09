#include "Line.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvpos main(VSInputPos vsInput)
{
	// 法線にワールド行列によるスケーリング・回転を適用		
    float4 vertexPos = mul(mul(viewMat, worldMat), vsInput.pos);

    VSOutputSvpos output; // ピクセルシェーダーに渡す値
    output.svpos = vertexPos;
    return output;
}