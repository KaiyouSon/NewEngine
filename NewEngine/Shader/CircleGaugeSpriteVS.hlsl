#include "CircleGaugeSprite.hlsli"
#include "ShaderIO.hlsli"

VSOutputSvposUv main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutputSvposUv output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mat, pos); // ���W�ɍs�����Z
    output.uv = uv;
    return output;
}