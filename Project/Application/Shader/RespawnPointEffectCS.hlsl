struct ParticleData
{
    float3 curPos;
    float2 curScale;
    float curRot;
    float curShininess;
    float4 curColor;
};

static const uint MaxNum = 32;
RWStructuredBuffer<ParticleData> inputData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    // 出力データを設定
    for (uint i = 0; i < MaxNum; i++)
    {
        ParticleData result = inputData[i];
        result.curPos.x = 0.f;
        result.curPos.y = 5.f + i * 1.f;
        result.curPos.z += 0.001f;
        
        result.curScale = 0.5f;
        result.curRot = 0.f;
        result.curShininess = 2.f;
        result.curColor = float4(1, 1, 1, 1);
        
        // 出力データを書き込む
        inputData[i] = result;
    }
}