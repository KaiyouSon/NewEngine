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
    float3 generatePos;
    float2 scale;
    float scaleAccel;
    float shininess;
    float4 color;
    float timer;
    float3 moveVec;
    float moveSpeed;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float2 seed = float2(DTid.x, DTid.x / 2.4124f);
            
    // パーティクルの設定
    ParticleData result = outputData[DTid.x];

    result.timer += 1;
    if (result.timer >= 360)
    {
        result.timer = 0;
    }
    result.pos += result.moveVec * result.moveSpeed;
    
    result.scale -= result.scaleAccel;
    if (result.scale.x <= 0)
    {
        result.pos = result.generatePos;
        result.scale = 0.05f + Random01(seed / 1000 + 2) * 0.15f;
        result.scaleAccel = 0.0002f + Random01(seed / 1000 + 4.123f) * 0.0004f;
        result.shininess = 0.1f + Random01(seed / 100 + 3) * 0.9f;
        
        seed = RandomSeed(seed, float2(DTid.x, DTid.x / 3.1415926f));
        result.moveVec.x = 1 - Random01(seed) * 2;
        seed = RandomSeed(seed, float2(DTid.x, DTid.x / 3.1415926f));
        result.moveVec.y = 1 - Random01(seed) * 2;
        seed = RandomSeed(seed, float2(DTid.x, DTid.x / 3.1415926f));
        result.moveVec.z = 1 - Random01(seed) * 2;
        result.moveVec = normalize(result.moveVec);

        result.moveSpeed = 0.0025f + Random01(seed / 10000) * 0.005f;
        result.color.a = 0;
    }
    
    result.color.a += 0.01f;
    if (result.color.a >= 1)
    {
        result.color.a = 1;

    }
        
    result.shininess += sin(result.timer * 3.1415926f / 180.f) * 0.0075f;
    outputData[DTid.x] = result;
}