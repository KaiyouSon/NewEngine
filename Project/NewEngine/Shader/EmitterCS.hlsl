#include "Emitter.hlsli"

struct InputData
{
    float3 curPos;
    float2 curScale;
    float curRot;
    float curShininess;
    float4 curColor;
};

struct OutputData
{
    float pos;
};

StructuredBuffer<OutputData> inputData : register(t0);
RWStructuredBuffer<OutputData> outputData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    outputData[0].pos = 100.f;
}