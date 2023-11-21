#include "Grass.hlsli"

// 四角形の頂点数
static const uint num = 4;

// 中心からのオフセット
static float4 offsetArray[num] =
{
    float4(-0.5f, 0.0f, 0, 0), //左下
    float4(-0.5f, 1.0f, 0, 0), //左上
    float4(+0.5f, 0.0f, 0, 0), //右下
    float4(+0.5f, 1.0f, 0, 0), //右上
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
    
    const float PI = 3.141592;
    
    offsetArray[1].x += sin(input[0].timer.x * PI / 180) / 3;
    offsetArray[3].x += sin(input[0].timer.x * PI / 180) / 3;
    for (uint i = 0; i < num; i++)
    {
        // 中心からのオフセットをスケーリング
        float4 voffset = offsetArray[i] * float4(input[0].scale, 0, 0);
        
        voffset = mul(billboardMat, voffset);
        
        float4 vpos = input[0].pos + voffset;
        
        element.pos = mul(mul(viewProjMat, worldMat), vpos);
        element.spos = mul(mul(lightViewProjMat, worldMat), vpos);
        element.uv = uvArray[i];
        output.Append(element);
    }
}
