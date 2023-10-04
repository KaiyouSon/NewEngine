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
    float3 curPos;
    float2 curScale;
    float curRot;
    float curShininess;
    float4 curColor;
};

StructuredBuffer<OutputData> inputData : register(t0);
RWStructuredBuffer<OutputData> outputData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    // 例: 出力データを設定
    OutputData result;
    result.curPos = float3(10, 10, 10); // 10.0fのベクトルを設定
    result.curScale = 1;
    result.curRot = 0;
    result.curShininess = 1;
    result.curColor = float4(1, 1, 1, 1);
    
    //result.curPos.z++;
    
    // 出力データを書き込む
    outputData[0] = result;
    
    //outputData[0].curPos = 10.f;
}