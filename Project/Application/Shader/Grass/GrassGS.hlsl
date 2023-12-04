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

static const float3 normalArray[num] =
{
    float3(+1.f, +1.f, 0.f), //左下
    float3(+1.f, -1.f, 0.f), //左上
    float3(-1.f, +1.f, 0.f), //右下
    float3(-0.5f, -1.f, 0.f), //右上
};

void CaluTangentAndBinormal(float3 normal, out float3 tangent, out float3 binormal);
float Random01(float2 seed);

[maxvertexcount(num)]
void main(point V2G input[1] : SV_POSITION, inout TriangleStream<G2P> output)
{
    G2P element = (G2P) 0;
    
    const float PI = 3.141592;
    
    //offsetArray[1].x += sin(input[0].timer.x * PI / 180) / 3;
    //offsetArray[3].x += sin(input[0].timer.x * PI / 180) / 3;
    //
    //float offsetZ = 0.5f - Random01(float2(input[0].timer.x, input[0].pos.x));
    //offsetArray[1].z += sin(input[0].timer.x * PI / 180);
    //offsetArray[3].z -= sin(input[0].timer.x * PI / 180);
    
    for (uint i = 0; i < num; i++)
    {
        // 中心からのオフセットをスケーリング
        float4 voffset = offsetArray[i] * float4(input[0].scale, 1, 0);
        voffset = mul(billboardMat, voffset);
        
        // 座標関連
        float4 vpos = input[0].pos + voffset;
        float4 wpos = mul(worldMat, vpos);
        element.pos = mul(viewProjMat, wpos);
        element.wpos = wpos;
        element.spos = mul(lightViewProjMat, wpos);

        // 法線関連
        float4 vnormal = normalize(mul(worldMat, mul(billboardMat, float4(normalArray[i], 0)))); // 頂点法線
        float3 pnormal = normalize(mul(worldMat, mul(billboardMat, float4(0, 0, -1, 0))));
        element.vnormal = vnormal;
        element.pnormal = pnormal;
        
        // uv
        element.uv = uvArray[i];
        
        // タンジェント関連
        float3 tangent, binormal;
        CaluTangentAndBinormal(pnormal, tangent, binormal);
        element.tangent = tangent;
        element.binormal = binormal;
        
        output.Append(element);
    }
}

// タンジェントと従法線
void CaluTangentAndBinormal(float3 normal, out float3 tangent, out float3 binormal)
{
    float3 up = float3(0, 1, 0);

    tangent = cross(normal, up);
    if (length(tangent) < 0.1)
    {
        tangent = cross(normal, float3(1, 0, 0));
    }

    tangent = normalize(tangent);
    binormal = cross(tangent, normal);
}

// 疑似乱数
float Random01(float2 seed)
{
    float result = frac(sin(dot(seed, float2(12.9898f, 78.223f))) * 43758.5453f);
    return result;
}
