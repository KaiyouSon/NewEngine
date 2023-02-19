#include "CircleGaugeSprite.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float Float2Cross(float2 v1, float2 v2);

float4 main(VSOutputSvposUv vsOutput) : SV_TARGET
{   
    const float PI = 3.14159f;
    
    float2 center = 0.5f;

    float2 v1 = { cos(startRadian), sin(startRadian) }; // �n�_�̃x�N�g��
    float2 v2 = { cos(endRadian), sin(endRadian) }; // �I�_�̃x�N�g��
    float2 v3 = vsOutput.uv - center; // �e�s�N�Z���̃x�N�g��
    
    // ���K��
    v1 = normalize(v1);
    v2 = normalize(v2);
    v3 = normalize(v3);
    
    float cross1 = Float2Cross(v1, v3); // �O�ρi���E����p�j
    if (endRadian - startRadian <= radians(180))
    {
        float dot1 = dot(v1, v2); // �n�_�̃x�N�g���ƏI�_�̃x�N�g���̓���
        float dot2 = dot(v1, v3); // �n�_�̃x�N�g���Ɗe�s�N�Z���̃x�N�g���̓���
        
        // �s�N�Z���̃x�N�g���̊p�x���A�I�_�̃x�N�g���̊p�x��菬������
        if (dot1 <= dot2)
        {
            // �s�N�Z���̍��W���n�_�̃x�N�g���̉E���ɂ��鎞
            if (cross1 >= 0.001)
            {
                clip(-1);
            }
        }
    }
    else // 180�x���������̏���
    {
        float dot1 = dot(-v1, v2); // [180�x���n�_�Ƃ����̃x�N�g��]��[�I�_�̃x�N�g��]�̓���
        float dot2 = dot(-v1, v3); // [180�x���n�_�Ƃ����̃x�N�g��]��[�e�s�N�Z���̃x�N�g��]�̓���
        
        float cross2 = Float2Cross(-v1, v3); // �O�ρi���E����p�j
        
        // �s�N�Z���̍��W���n�_�̃x�N�g���̉E���ɂ��鎞
        if (dot1 <= dot2)
        {
            // [�s�N�Z���̍��W]��[180�x���n�_�Ƃ����̃x�N�g��]�̉E���ɂ��鎞
            if (cross2 >= 0)
            {
                clip(-1);
            }
        }
        
        if (cross1 >= 0 && cross2 <= 0)
        {
            clip(-1);
        }
    }
    
    return float4(tex.Sample(smp, vsOutput.uv)) * color;
}

float Float2Cross(float2 v1, float2 v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}