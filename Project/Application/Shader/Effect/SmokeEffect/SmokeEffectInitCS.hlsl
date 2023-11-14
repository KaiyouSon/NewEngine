struct ParticleData
{
    float3 pos;
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float4 color;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

struct SmokeEffect
{
    uint timer;
    uint maxTimer;
    float timerRate;
    uint index;
};
RWStructuredBuffer<SmokeEffect> smokeEffect : register(u1);

// パーティクルの最大数
cbuffer ConstantBufferMaxParticleData : register(b0)
{
    uint max;
}

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x == 0)
    {
        smokeEffect[0].timer = 0;
        smokeEffect[0].maxTimer = 10;
        smokeEffect[0].timerRate = 0;
        smokeEffect[0].index = 0;
    }
}
