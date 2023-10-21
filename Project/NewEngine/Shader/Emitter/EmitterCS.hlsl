#include "Emitter.hlsli"

struct ArrayData
{
    float3 curPos;
    float2 curScale;
    float curRot;
    float curShininess;
    float4 curColor;
};

struct Data
{
    ArrayData data[2];
};

RWStructuredBuffer<Data> outputData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    Data result = outputData[0];
    // 例: 出力データを設定
    for (uint i = 0; i < 2; i++)
    {
        result.data[i].curPos.y = 5.f + i * 5.f;
        result.data[i].curPos.z += 0.001f;
    }
    // 出力データを書き込む
    outputData[0] = result;
}
