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

// �e�N�X�`���[�̖ʐ�
cbuffer ConstantBufferTextureSizeData : register(b0)
{
    float2 size;
    float area;
}

cbuffer ConstantBufferIsExplosionFlag : register(b1)
{
    uint isExplosion;
}

struct ParticleData
{
    float3 pos;
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float shininess;
    float4 color;
    float timer;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

Texture2D<float4> tex : register(t0);

[numthreads(1000, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    // area == 10w�ɂȂ��Ă��邩��A[numthreads(1000, 1, 1)]����1CS��100��for����
    
    // 100�̃f�[�^���������邽�߂̃C���f�b�N�X���v�Z
    uint dataPerThread = area / 1000;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;
    
    float2 offsetPos = 0;
    float2 seed = 0;
    
    for (uint i = startIndex; i < endIndex; i++)
    {
        ParticleData result = outputData[i];
        
        // �C���f�b�N�X(x,y)
        uint2 index = uint2((startIndex + i) % 256, (startIndex + i) / 256);
        
        // �V�[�g������
        seed.x = index.x;
        seed.y = index.y;
        
        // ���W
        seed = RandomSeed(seed, index);
        result.pos.x = Random01(seed) * size.x;
        seed = RandomSeed(seed, index);
        result.pos.y = Random01(seed) * size.y;

        // �ړ��x�N�g��
        seed = RandomSeed(seed, index);
        result.moveVec.x = 1 - Random01(seed) * 2;
        seed = RandomSeed(seed, index);
        result.moveVec.y = 1 - Random01(seed) * 2;
        seed = RandomSeed(seed, index);
        result.moveVec.z = 1 - Random01(seed) * 2;
        
        // �ړ����x
        const float baseSpeed = 4.f;
        const float rate = 2.f;
        seed = RandomSeed(seed, index);
        result.moveAccel.x = baseSpeed + Random01(seed) * rate;
        seed = RandomSeed(seed, index);
        result.moveAccel.y = baseSpeed + Random01(seed) * rate;
        seed = RandomSeed(seed, index);
        result.moveAccel.z = baseSpeed + Random01(seed) * rate;
        
        // �X�P�[��
        seed = RandomSeed(seed, index);
        result.scale = 1.f + Random01(seed) * 1.f;
        
        // �P�x
        seed = RandomSeed(seed, index);
        result.shininess = 1.f;
        
        // �F
        result.color = tex.Load(int3(result.pos.x, result.pos.y, 0));
        
        // �^�C�}�[
        seed = RandomSeed(seed, index);
        result.timer = Random01(seed) * 360;
        
        // ���S��
        float2 offsetPos = -size / 2.f;
        result.pos.x += offsetPos.x;
        result.pos.y += offsetPos.y;
        
        // �o�̓f�[�^����������
        outputData[i] = result;
    }
}