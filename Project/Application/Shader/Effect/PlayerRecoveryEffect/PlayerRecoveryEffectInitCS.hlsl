// パーティクルの最大数
cbuffer ConstantBufferMaxParticleData : register(b0)
{
    uint max;
}

// パーティクルの設定データ
cbuffer ConstantBufferPlayerRecoveryEffectData : register(b1)
{
    float3 baseSpeed;
    float3 rateSpeed;
    float3 baseScale;
    float3 rateScale;
    float3 baseScaleAccel;
    float3 rateScaleAccel;
    float4 baseColor;
};

struct ParticleData
{
    float3 pos;
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float scaleAccel;
    float shininess;
    float4 color;
    float alphaAccel;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

// 疑似乱数
float Random01(float2 seed);

// 疑似乱数シード
float2 RandomSeed(float2 seed, uint2 index);

[numthreads(50, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint dataPerThread = max / 50;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;
    
    for (uint i = startIndex; i < endIndex; i++)
    {
        // インデックス(x,y)
        float2 seed = uint2(i % 256, i / 256) + uint2(-1, 1);

        ParticleData result = outputData[i];
        
        // 座標
        float3 basePos = float3(2.f, 2.5f, 2.f); // -basePos ~ basePos
        float3 ratePos = basePos * 2;
        seed = RandomSeed(seed, i);
        result.pos.x = basePos.x - Random01(seed) * ratePos.x;
        seed = RandomSeed(seed, i);
        result.pos.y = basePos.y - Random01(seed) * ratePos.y;
        seed = RandomSeed(seed, i);
        result.pos.z = basePos.z - Random01(seed) * ratePos.z;
        
        // ベクトル
        result.moveVec = normalize(float3(1.f, 1.f, 1.f));
        
        // 移動速度
        seed = RandomSeed(seed, i);
        result.moveAccel.x = baseSpeed.x - Random01(seed) * rateSpeed.x;
        seed = RandomSeed(seed, i);
        result.moveAccel.y = baseSpeed.y - Random01(seed) * rateSpeed.y;
        seed = RandomSeed(seed, i);
        result.moveAccel.z = baseSpeed.z - Random01(seed) * rateSpeed.z;
        
        // スケール
        seed = RandomSeed(seed, i);
        result.scale = baseScale + Random01(seed) * rateScale;
        
        // スケール変化速度
        seed = RandomSeed(seed, i);
        result.scaleAccel = baseScaleAccel + Random01(seed) * rateScaleAccel;
        
        // 輝度
        seed = RandomSeed(seed, i);
        result.shininess = 0.5f + Random01(seed) * 0.5f;
        
        // 色
        result.color = float4(0.76f, 0.76f, 0.47f, 0.f);
        
        // アルファ変化速度
        seed = RandomSeed(seed, i);
        result.alphaAccel = 0.05f + Random01(seed) * 0.05f;
        
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