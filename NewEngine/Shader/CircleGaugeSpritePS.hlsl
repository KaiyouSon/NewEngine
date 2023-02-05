#include "CircleGaugeSpriteH.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    const float PI = 3.14159f;
    
    float2 center = 0.5f;
    
    // ��x�N�g��
    float2 v1 = { cos(startRadian), sin(startRadian) };
    v1 = normalize(v1);
    
    float rad1 = atan2(v1.y, v1.x);
    if (rad1 < 0)
    {
        rad1 = atan2(v1.y, v1.x) + radians(360);
    }
    else if (rad1 < radians(360))
    {
        rad1 = atan2(v1.y, v1.x) - radians(360);
    }
    
    // uv���W����̃x�N�g��
    float2 v2 = input.uv - center;
    v2 = normalize(v2);
    float rad2 = atan2(v2.y, v2.x);
    if (rad2 < 0)
    {
        rad2 = atan2(v2.y, v2.x) + radians(360);
    }
    else if (rad2 < radians(360))
    {
        rad2 = atan2(v2.y, v2.x) - radians(360);
    }
    
    // ��]�x�N�g��
    float2 v3 = { cos(endRadian + rad1), sin(endRadian + rad1) };
    v3 = normalize(v3);
    float rad3 = atan2(v3.y, v3.x);
    if (rad3 < 0)
    {
        rad3 = atan2(v3.y, v3.x) + radians(360);
    }
    else if (rad3 < radians(360))
    {
        rad3 = atan2(v3.y, v3.x) - radians(360);
    }
    
    // �s�N�Z����j�����鏈��
    if (rad2 > rad1 && rad2 < rad3)
    {
        clip(-1);
    }

    return float4(tex.Sample(smp, input.uv)) * color;
}