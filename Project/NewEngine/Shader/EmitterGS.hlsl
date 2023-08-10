#include "Emitter.hlsli"

// �l�p�`�̒��_��
static const uint num = 4;

// ���S����̃I�t�Z�b�g
static const float4 offsetArray[num] =
{
    float4(-0.5f, -0.5f, 0, 0), //����
    float4(-0.5f, +0.5f, 0, 0), //����
    float4(+0.5f, -0.5f, 0, 0), //�E��
    float4(+0.5f, +0.5f, 0, 0), //�E��
};

static const float2 uvArray[num] =
{
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 1.0f), //�E��
    float2(1.0f, 0.0f), //�E��
};

[maxvertexcount(num)]
void main(point V2G input[1] : SV_POSITION, inout TriangleStream<G2P> output)
{
    G2P element = (G2P) 0;
    
    for (uint i = 0; i < num; i++)
    {
        // ���S����̃I�t�Z�b�g���X�P�[�����O
        float4 voffset = offsetArray[i] * float4(input[0].scale, 0, 0);
        
        // Z����]�s��
        matrix rotZMat =
        {
            cos(input[0].rot), sin(input[0].rot), 0, 0,
            -sin(input[0].rot), cos(input[0].rot), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        
        voffset = mul(billboardMat, voffset);
        voffset = mul(rotZMat, voffset);
        //voffset = mul(billboardMat, voffset);
        
        float4 vpos = input[0].pos + voffset;
        
        element.pos = mul(mul(viewMat, worldMat), vpos);
        element.uv = uvArray[i];
        element.shininess = input[0].shininess;
        element.color = input[0].color;
        output.Append(element);
    }
}