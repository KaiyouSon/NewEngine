#include "Object3D.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvposPosNormalUv main(VSInputPosNormalUv vsInput)
{
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
    float4 wnormal = normalize(mul(worldMat, float4(vsInput.normal, 0)));
    float4 wpos = mul(worldMat, vsInput.pos);
    float4 vertexPos = mul(mul(viewMat, worldMat), vsInput.pos);

    // �s�N�Z���V�F�[�_�[�ɓn���l
    VSOutputSvposPosNormalUv output = (VSOutputSvposPosNormalUv) 0;
    output.svpos = vertexPos;
    output.worldPos = wpos;
    output.normal = wnormal.xyz;
    output.uv = vsInput.uv;

    return output;
}