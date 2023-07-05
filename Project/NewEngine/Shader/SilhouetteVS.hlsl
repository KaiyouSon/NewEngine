#include "Silhouette.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvposNormalUv main(VSInputPosNormalUv vsInput)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
    float4 wnormal = normalize(mul(worldMat, float4(vsInput.normal, 0)));

    matrix mat = mul(viewMat, worldMat);

    VSOutputSvposNormalUv output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mat, vsInput.pos);
    output.normal = wnormal.xyz;
    output.uv = vsInput.uv;

    return output;
}