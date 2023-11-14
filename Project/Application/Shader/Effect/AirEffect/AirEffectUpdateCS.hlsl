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

struct AirEffect
{
    uint timer;
    uint maxTimer;
    float timerRate;
    uint index;
};
RWStructuredBuffer<AirEffect> airEffect : register(u1);

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
        airEffect[0].timer++;
        if (airEffect[0].timer >= airEffect[0].maxTimer)
        {
            // ������
            for (uint i = 0; i < 100; i++)
            {
                outputData[airEffect[0].index] = InitParticleData(airEffect[0].index);
                airEffect[0].index++;
            }
            
            airEffect[0].timer = 0;
            if (airEffect[0].index >= max)
            {
                airEffect[0].index = 0;
            }
        }
    }
    
    
    // area == 10w�ɂȂ��Ă��邩��A[numthreads(1000, 1, 1)]����1CS��100��for����
    
    // 100�̃f�[�^���������邽�߂̃C���f�b�N�X���v�Z
    uint dataPerThread = max / 1000;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;

    for (uint i = startIndex; i < endIndex; i++)
    {
        ParticleData result = outputData[i];
        
        // �X�P�[����0�̏ꍇ
        if (result.scale.x <= 0)
        {
            result.scale = 0;
            continue;
        }
        
        // ���W
        result.pos += result.moveVec * result.moveAccel;
        
        result.color.a += 0.01f;
        if (result.color.a >= 1)
        {
            result.color.a = 1;
            result.scale -= 0.001f;
        }
        
        // �o�̓f�[�^����������
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
    const float3 basePos = float3(100, 10, 100);
    const float3 ratePos = float3(200, 70, 200);
    seed = RandomSeed(seed, index);
    result.pos.x = basePos.x - Random01(seed) * ratePos.x;
    seed = RandomSeed(seed, index);
    result.pos.y = basePos.y - Random01(seed) * ratePos.y;
    seed = RandomSeed(seed, index);
    result.pos.z = basePos.z - Random01(seed) * ratePos.z;

    // �x�N�g��
    result.moveVec = normalize(float3(1, 1, 1));
        
    // �ړ����x
    const float3 baseSpeed = float3(0.005f, -0.01f, 0.005f);
    const float3 rateSpeed = float3(0.01f, -0.005f, 0.01f);
    seed = RandomSeed(seed, index);
    result.moveAccel.x = baseSpeed.x - Random01(seed) * rateSpeed.x;
    seed = RandomSeed(seed, index);
    result.moveAccel.y = baseSpeed.y + Random01(seed) * rateSpeed.y;
    seed = RandomSeed(seed, index);
    result.moveAccel.z = baseSpeed.z - Random01(seed) * rateSpeed.z;
        
    // �X�P�[��
    seed = RandomSeed(seed, index);
    result.scale = 0.35f + Random01(seed) * 0.35f;
    
    // �P�x
    seed = RandomSeed(seed, index);
    result.shininess = 1.0f;
    //0.75f + Random01(seed) * 0.75f;
        
    // �F
    result.color = float4(0.9f, 0.7f, 0.37f, 0.f);
    
    return result;
}