struct ArrayData
{
    float3 curPos;
    float2 curScale;
    float curRot;
    float curShininess;
    float4 curColor;
};

static const uint MaxNum = 32;
struct Data
{
    ArrayData data[MaxNum];
};

StructuredBuffer<Data> inputData : register(t0);
RWStructuredBuffer<Data> outputData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    Data result = inputData[0];
    // 例: 出力データを設定
    for (uint i = 0; i < MaxNum; i++)
    {
        result.data[i].curPos.y = 5.f + i * 1.f;
        result.data[i].curPos.z += 0.001f;
        
        result.data[i].curScale = 0.5f;
        result.data[i].curShininess = 2.f;
        result.data[i].curColor = float4(1, 1, 1, 1);
    }
    // 出力データを書き込む
    outputData[0] = result;
}