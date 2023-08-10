#include "Emitter.hlsli"

// 四角形の頂点数
static const uint num = 4;

// 中心からのオフセット
static const float4 offsetArray[num] =
{
    float4(-0.5f, -0.5f, 0, 0), //左下
    float4(-0.5f, +0.5f, 0, 0), //左上
    float4(+0.5f, -0.5f, 0, 0), //右下
    float4(+0.5f, +0.5f, 0, 0), //右上
};

static const float2 uvArray[num] =
{
    float2(0.0f, 1.0f), //左下
    float2(0.0f, 0.0f), //左上
    float2(1.0f, 1.0f), //右下
    float2(1.0f, 0.0f), //右上
};

[maxvertexcount(num)]
void main(point V2G input[1] : SV_POSITION, inout TriangleStream<G2P> output)
{
    G2P element = (G2P) 0;
    
    for (uint i = 0; i < num; i++)
    {
        // 中心からのオフセットをスケーリング
        float4 voffset = offsetArray[i] * float4(input[0].scale, 0, 0);
        
        // Z軸回転行列
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