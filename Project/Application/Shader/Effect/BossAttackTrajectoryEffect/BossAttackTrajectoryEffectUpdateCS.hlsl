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

// �x�N�g���̃f�[�^
cbuffer ConstantBufferVecData : register(b1)
{
    uint isActive;
    float3 startPos;
    float3 endPos;
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
    // �^�C�}�[�̏���
    if (DTid.x == 0 && isActive == true)
    {
        bossAttackTrajectoryEffect[0].timer++;
        if (bossAttackTrajectoryEffect[0].timer >= bossAttackTrajectoryEffect[0].maxTimer)
        {
            for (uint i = 0; i < 30; i++)
            {
                uint index = bossAttackTrajectoryEffect[0].index;
                outputData[index] = InitParticleData(index);
                bossAttackTrajectoryEffect[0].index++;
            }
            
            bossAttackTrajectoryEffect[0].timer = 0;
            if (bossAttackTrajectoryEffect[0].index >= max)
            {
                bossAttackTrajectoryEffect[0].index = 0;
            }
        }
    }
    
    uint dataPerThread = max / 1000;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;

    for (uint i = startIndex; i < endIndex; i++)
    {
        ParticleData result = outputData[i];
        
        if (result.scale.x <= 0)
        {
            continue;
        }
        
        result.pos += result.moveVec * result.moveAccel;
        
        result.shininess += 0.01f;
        result.color.a += 0.1f;
        if (result.color.a >= 1)
        {
            result.color.a = 1;
            result.scale -= 0.0025f;
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
    float3 vec = endPos - startPos;
    float3 frontVec = normalize(vec);
    float3 rightVec = normalize(cross(frontVec, float3(0, 1, 0)));
    float3 upVec = normalize(cross(frontVec, rightVec));
    
    float sign = 0;
    
    // ���^���ʂɐL�΂�
    seed = RandomSeed(seed, index);
    sign = Random01(seed) > 0.5f ? +1 : -1;
    
    float l = Random01(seed) * length(vec);
    
    seed = RandomSeed(seed, index);
    result.pos = startPos + frontVec * l;

    // �㉺���E�ɂ��炷
    seed = RandomSeed(seed, index);
    sign = Random01(seed) > 0.5f ? +1 : -1;
    
    seed = RandomSeed(seed, index);
    result.pos += rightVec * Random01(seed) * sign;
    
    seed = RandomSeed(seed, index);
    sign = Random01(seed) > 0.5f ? +1 : -1;
    
    seed = RandomSeed(seed, index);
    result.pos += upVec * Random01(seed) * 2.f;

    // �x�N�g��
    result.moveVec = frontVec;
        
    // �ړ����x
    seed = RandomSeed(seed, index);
    result.moveAccel = 0.005f + Random01(seed) * 0.01f;
        
    // �X�P�[��
    float baseScale = smoothstep(0, 1, l) - 0.3f;
    
    seed = RandomSeed(seed, index);
    result.scale = baseScale + Random01(seed) * 0.25f;
    
    // �P�x
    result.shininess = 5.0f;
        
    // �F
    result.color = float4(0.53f, 0.f, 0.f, 0.f);
    
    return result;
}