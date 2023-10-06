#include "OutLine.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvpos main(VSInputPosNormal vsInput)
{
    // ワールド行列とビュー行列をかけた    
    matrix mat = mul(viewMat, worldMat);
    float4 pos = vsInput.pos + float4(vsInput.normal, 0) * 0.1f;
    
    // 出力構造体
    VSOutputSvpos output; 
    output.svpos = mul(mat, pos);

    return output;
}
