// パーティクルの最大数
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

// 疑似乱数
float Random01(float2 seed);

// 疑似乱数シード
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
        
        // インデックス(x,y)
        float2 seed = uint2(i % 256, i / 256) + uint2(-1, 1);

        ParticleData result = outputData[i];
        
        // 座標
        float radius = 0.25f;
        
        // ランダムな球面座標を生成
        seed = RandomSeed(seed, i);
        float theta = 2 * 3.14159265359 * Random01(seed);
        seed = RandomSeed(seed, i);
        float phi = acos(2 * Random01(seed) - 1);
        
        // 極座標をデカルト座標に変換して球の表面上の点を取得
        result.pos.x = radius * sin(phi) * cos(theta);
        result.pos.y = radius * sin(phi) * sin(theta);
        result.pos.z = radius * cos(phi);

        // ベクトル
        result.moveVec = normalize(result.pos);
        
        // 移動速度
        float baseSpeed = 1.f;
        seed = RandomSeed(seed, i);
        result.moveAccel = baseSpeed;
        
        // スケール
        seed = RandomSeed(seed, i);
        result.scale = 0.25f + Random01(seed) * 0.25f;
        
        // 輝度
        seed = RandomSeed(seed, i);
        result.shininess = 0.5f + Random01(seed) * 0.5f;
        
        // 色
        seed = RandomSeed(seed, i);
        result.color = float4(0.6f, 0.f, 0.f, 1.f);
        result.color.r -= Random01(seed) * 0.3f;
        outputData[i] = result;
    }
}

// 疑似乱数
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