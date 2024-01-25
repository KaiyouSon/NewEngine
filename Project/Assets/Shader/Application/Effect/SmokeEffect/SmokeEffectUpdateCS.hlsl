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

// �p�[�e�B�N���̍ő吔
cbuffer ConstantBufferMaxParticleData : register(b0)
{
    uint max;
}

// �x�N�g���̃f�[�^
cbuffer ConstantBufferEmitterData : register(b1)
{
    uint isGenerate;
    float3 generatePos;
    uint generateTimer;
    float3 generateRange;
    uint generateNum;
}

// �^������
float Random01(float2 seed);

// �^�������V�[�h
float2 RandomSeed(float2 seed, uint2 index);

// ������
ParticleData InitParticleData(uint index);

[numthreads(64, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    // �^�C�}�[�̏���
    if (DTid.x == 0 && isGenerate == true)
    {
        smokeEffect[0].timer++;
        if (smokeEffect[0].timer >= generateTimer)
        {
            for (uint i = 0; i < generateNum; i++)
            {
                uint index = smokeEffect[0].index;
                outputData[index] = InitParticleData(index);
                smokeEffect[0].index++;
            }
            
            smokeEffect[0].timer = 0;
            if (smokeEffect[0].index >= max)
            {
                smokeEffect[0].index = 0;
            }
        }
    }
    
    uint dataPerThread = max / 64;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;

    for (uint i = startIndex; i < endIndex; i++)
    {
        ParticleData result = outputData[i];
        
        if (result.scale.x <= 0)
        {
            result.scale = 0;
            outputData[i] = result;
            continue;
        }
        
        result.pos += result.moveVec * result.moveAccel;
        
        result.color.a += 0.1f;
        if (result.color.a >= 0.5f)
        {
            result.color.a = 0.5f;
            result.scale -= 0.1f;
        }
        
        outputData[i] = result;
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

// ������
ParticleData InitParticleData(uint index)
{
    ParticleData result;
    
    // �C���f�b�N�X(x,y)
    float2 seed = uint2(index % 256, index / 256) + uint2(-1, 1);

    // ���W
    result.pos = generatePos;
    seed = RandomSeed(seed, index);
    result.pos.x += generateRange.x - Random01(seed) * generateRange.x * 2;
    seed = RandomSeed(seed, index);
    result.pos.z += generateRange.z - Random01(seed) * generateRange.z * 2;
    
    // �x�N�g��
    result.moveVec = float3(0, 1, 0);
        
    // �ړ����x
    seed = RandomSeed(seed, index);
    result.moveAccel = 0.05f + Random01(seed) * 0.05f;
    
    // �X�P�[��
    float baseScale = 1.1f;
    seed = RandomSeed(seed, index);
    result.scale = baseScale + Random01(seed) * 0.9f;
    
    // �F
    seed = RandomSeed(seed, index);
    result.color = float4(1.f, 1.f, 1.f, 0.1f);
    
    return result;
}