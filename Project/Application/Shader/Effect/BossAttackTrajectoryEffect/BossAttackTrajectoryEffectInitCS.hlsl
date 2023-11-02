struct ParticleData
{
    float3 pos;
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float shininess;
    float4 color;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

struct BossAttackTrajectoryEffect
{
    uint timer;
    uint maxTimer;
    float timerRate;
    uint index;
};
RWStructuredBuffer<BossAttackTrajectoryEffect> bossAttackTrajectoryEffect : register(u1);

// �p�[�e�B�N���̍ő吔
cbuffer ConstantBufferMaxParticleData : register(b0)
{
    uint max;
}

// �^������
float Random01(float2 seed);

// �^�������V�[�h
float2 RandomSeed(float2 seed, uint2 index);

// ������
ParticleData InitParticleData(uint index);

[numthreads(1000, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x == 0)
    {
        bossAttackTrajectoryEffect[0].timer = 0;
        bossAttackTrajectoryEffect[0].maxTimer = 1;
        bossAttackTrajectoryEffect[0].timerRate = 0;
        bossAttackTrajectoryEffect[0].index = 0;
    }
}

// �^������
float Random01(float2 seed)
{
    float result = frac(sin(dot(seed, float2(12.9898f, 78.223f))) * 43758.5453f);
    return result;
}

// �^�������V�[�h
float2 RandomSeed(float2 seed, uint2 index)
{
    float random = Random01(seed);
    float2 result = float2(random / index.x, random * index.y);
    return result;
}