#include "OutLine.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvpos main(VSInputPosNormal vsInput)
{
    // ���[���h�s��ƃr���[�s���������    
    matrix mat = mul(viewMat, worldMat);
    float4 pos = vsInput.pos + float4(vsInput.normal, 0) * 0.1f;
    
    // �o�͍\����
    VSOutputSvpos output; 
    output.svpos = mul(mat, pos);

    return output;
}