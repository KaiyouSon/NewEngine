#include "ToonRender.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvposPosNormalUv main(VSInputPosNormalUv vsInput)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
    float4 wnormal = normalize(mul(worldMat, float4(vsInput.normal, 0)));
    float4 wpos = mul(worldMat, vsInput.pos);

    VSOutputSvposPosNormalUv output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mul(viewMat, worldMat), vsInput.pos);
    output.worldPos = wpos;
    output.normal = wnormal.xyz;
    output.uv = vsInput.uv;

    return output;
}