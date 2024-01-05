#include "ToneMapping.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 ToneCurve(float4 inputColor, float gain, float offset)
{
    // ���̓s�N�Z����RGB�l���擾
    float3 rgb = inputColor.rgb;

    // �g�[���J�[�u�̌v�Z�i�e�`�����l���ɑ΂��ăQ�C���ƃI�t�Z�b�g��K�p�j
    rgb = rgb * gain + offset;

    // �V����RGB�l��Ԃ�
    return float4(rgb, inputColor.a);
}

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);

    // �g�[���J�[�u�̊֐����Ăяo���A���ʂ��o��
    return ToneCurve(texColor, gain, offset);

    //return float4(texColor.rgb, 1);

}