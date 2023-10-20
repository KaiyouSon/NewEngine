// パーティクルの最大数
cbuffer ConstantBufferMaxParticleData : register(b0)
{
    uint max;
}

// パーティクルのベースベクトル
cbuffer ConstantBufferVec : register(b1)
{
    float3 vec;
}

struct ParticleData
{
    float3 pos;
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float scaleAccel;
    float shininess;
    float4 color;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

struct LeadEffect
{
    uint timer;
    uint maxTimer;
    float timerRate;
    uint startIndex;
    uint endIndex;
};
RWStructuredBuffer<LeadEffect> leadEffect : register(u1);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x == 0)
    {
        leadEffect[0].timer = 0;
        leadEffect[0].maxTimer = 20;
        leadEffect[0].timerRate = 0;
        leadEffect[0].startIndex = 0;
        leadEffect[0].endIndex = 0;
    }
}
