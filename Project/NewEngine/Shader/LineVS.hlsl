#include "Line.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvpos main(VSInputPos vsInput)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
    float4 vertexPos = mul(mul(viewMat, worldMat), vsInput.pos);

    VSOutputSvpos output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = vertexPos;
    return output;
}