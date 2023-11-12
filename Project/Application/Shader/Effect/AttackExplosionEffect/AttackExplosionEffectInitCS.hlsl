// �p�[�e�B�N���̍ő吔
cbuffer ConstantBufferMaxParticleData : register(b0)
{
    uint max;
}

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

// �^������
float Random01(float2 seed);

// �^�������V�[�h
float2 RandomSeed(float2 seed, uint2 index);

[numthreads(100, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint dataPerThread = max / 100;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;
    
    for (uint i = startIndex; i < endIndex; i++)
    {
        if (i > max)
        {
            return;
        }
        
        // �C���f�b�N�X(x,y)
        float2 seed = uint2(i % 256, i / 256) + uint2(-1, 1);

        ParticleData result = outputData[i];
        
        // ���W
        float3 basePos = float3(4.f, 5.5f, 4.f); // -basePos ~ basePos
        float3 ratePos = basePos * 2;

        seed = RandomSeed(seed, i);
        result.pos.x = basePos.x - Random01(seed) * ratePos.x;
        seed = RandomSeed(seed, i);
        result.pos.y = basePos.y - Random01(seed) * ratePos.y;
        seed = RandomSeed(seed, i);
        result.pos.z = basePos.z - Random01(seed) * ratePos.z;
        
        // �x�N�g��
        result.moveVec = normalize(float3(1.f, 1.f, 1.f));
        
        // �ړ����x
        //seed = RandomSeed(seed, i);
        //result.moveAccel.x = baseSpeed.x - Random01(seed) * rateSpeed.x;
        //seed = RandomSeed(seed, i);
        //result.moveAccel.y = baseSpeed.y - Random01(seed) * rateSpeed.y;
        //seed = RandomSeed(seed, i);
        //result.moveAccel.z = baseSpeed.z - Random01(seed) * rateSpeed.z;
        
        // �X�P�[��
        seed = RandomSeed(seed, i);
        result.scale = 0.25f + Random01(seed) * 0.25f;
        
        // �P�x
        seed = RandomSeed(seed, i);
        result.shininess = 0.5f + Random01(seed) * 0.5f;
        
        // �F
        result.color = float4(0.76f, 0.76f, 0.47f, 0.f);
        
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