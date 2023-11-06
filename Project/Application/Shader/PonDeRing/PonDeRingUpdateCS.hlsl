// ランダムの数
float Random01(float2 seed)
{
    float result = frac(sin(dot(seed, float2(12.9898f, 78.223f))) * 43758.5453f);
    return result;
}

// 疑似乱数シード
float2 RandomSeed(float2 seed, uint2 index)
{
    float random = Random01(seed);
    float2 result = float2(random / index.x, random * index.y);
    return result;
}

// テクスチャーの面積
cbuffer ConstantBufferParticleObject : register(b0)
{
    uint vertexSize;
    uint meshParticleSize;
    uint maxParticleSize;
}

struct ModelData
{
    float3 pos;
    float3 normal;
    float2 uv;
    uint boneIndex[4];
    float boneWeight[4];
};
StructuredBuffer<ModelData> modelData : register(t0);

struct ParticleData
{
    float3 pos;
    float2 scale;
    float shininess;
    float4 color;
    float timer;
    float moveSpeed;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

[numthreads(100, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint pointIndexOffset = (DTid.x / 100) * 3;

    float2 seed = 0;
    
    uint dataPerThread = meshParticleSize / 100;
    
    uint start = (DTid.x) * dataPerThread;
    uint end = (DTid.x + 1) * dataPerThread;
        
    // パーティクルの設定
    for (uint i = start; i < end; i++)
    {
        if (i > maxParticleSize)
        {
            return;
        }
            
        ParticleData result = outputData[i];

        result.timer += 1;
        if (result.timer >= 360)
        {
            result.timer = 0;
        }
        result.pos.x += sin(result.timer * 3.1415926f / 180.f) * result.moveSpeed;
        result.pos.y += cos(result.timer * 3.1415926f / 180.f) * result.moveSpeed;
        
        result.shininess += sin(result.timer * 3.1415926f / 180.f) * 0.0075f;
        outputData[i] = result;
    }
}