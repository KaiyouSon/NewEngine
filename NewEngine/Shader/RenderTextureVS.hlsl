#include "RenderTexture.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvposUv main(VSInputPosUv vsInput)
{
    VSOutputSvposUv output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mat, vsInput.pos); // ���W�ɍs�����Z
    output.uv = vsInput.uv;
    return output;
}