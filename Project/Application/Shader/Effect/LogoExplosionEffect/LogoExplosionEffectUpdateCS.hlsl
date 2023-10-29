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
    
    for (uint i = startIndex; i < endIndex; i++)
    {
        ParticleData result = outputData[i];
        
        if (isExplosion == true)
        {
            if (result.scale.x <= 0)
            {
                continue;
            }
            // �ړ�
            result.pos += normalize(result.moveVec) * result.moveAccel;
            result.scale -= 0.01f;
            result.scale = max(result.scale, 0);
        
        }
        
        result.timer += 1;
        if (result.timer >= 360)
        {
            result.timer = 0;
        }
        
        result.pos.x += sin(result.timer * 3.1415926f / 180.f) * 0.01f;
        result.pos.z += cos(result.timer * 3.1415926f / 180.f) * 0.01f;
        
        result.shininess += sin(result.timer * 3.1415926f / 180.f) * 0.0075f;
        
        // �o�̓f�[�^����������
        outputData[i] = result;
    }
}
