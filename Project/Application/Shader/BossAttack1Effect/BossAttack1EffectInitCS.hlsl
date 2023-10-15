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

Texture2D<float4> tex : register(t0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float2 offsetPos = 0;
    float2 seed = 0;
    
    for (uint i = 0; i < area; i++)
    {
        ParticleData result = outputData[i];
        
        // �C���f�b�N�X(x,y)
        uint2 index = uint2(i % 256, i / 256);
        
        // �V�[�g������
        seed.x = index.x;
        seed.y = index.y;
        
        // ���W
        seed = RandomSeed(seed, index);
        result.pos.x = Random01(seed) * size.x;
        seed = RandomSeed(seed, index);
        result.pos.z = Random01(seed) * size.y;

        // �ړ��x�N�g��
        seed = RandomSeed(seed, index);
        result.moveVec.x = Random01(seed);
        seed = RandomSeed(seed, index);
        result.moveVec.y = Random01(seed);
        seed = RandomSeed(seed, index);
        result.moveVec.z = Random01(seed);
        
        // �ړ����x
        const float rate = 0.5f;
        seed = RandomSeed(seed, index);
        result.moveAccel.x = 5 + Random01(seed) * rate;
        seed = RandomSeed(seed, index);
        result.moveAccel.y = 5 + Random01(seed) * rate;
        seed = RandomSeed(seed, index);
        result.moveAccel.z = 5 + Random01(seed) * rate;
        // �X�P�[��
        seed = RandomSeed(seed, index);
        result.scale = 1.f + Random01(seed) * 1.f;
        
        // �P�x
        seed = RandomSeed(seed, index);
        result.shininess = 2.f + Random01(seed) * 1.f;
        
        // �F
        result.color = tex.Load(int3(result.pos.x, result.pos.z, 0));
        
        // ���S��
        float2 offsetPos = -size / 2.f;
        result.pos.x += offsetPos.x;
        result.pos.z += offsetPos.y;
        
        // �o�̓f�[�^����������
        outputData[i] = result;
    }
}
