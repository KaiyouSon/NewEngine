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

// パーティクルの最大数
cbuffer ConstantBufferMaxParticleData : register(b0)
{
    uint max;
}

// ベクトルのデータ
cbuffer ConstantBufferVecData : register(b1)
{
    float3 vec;
}

// 疑似乱数
float Random01(float2 seed);

// 疑似乱数シード
float2 RandomSeed(float2 seed, uint2 index);

// 初期化
ParticleData InitParticleData(uint index);

[numthreads(1000, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    // タイマーの処理
    if (DTid.x == 0)
    {
        bossAttackTrajectoryEffect[0].timer++;
        if (bossAttackTrajectoryEffect[0].timer >= bossAttackTrajectoryEffect[0].maxTimer)
        {
            for (uint i = 0; i < 50; i++)
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
        if (i >= bossAttackTrajectoryEffect[0].index)
        {
            return;
        }
        
        ParticleData result = outputData[i];
        
        if (result.scale.x <= 0)
        {
            continue;
        }
        
        result.shininess += 0.01f;
        result.color.a += 0.01f;
        if (result.color.a >= 0)
        {
            result.color.a = 1;
            result.scale -= 0.001f;
        }
        
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

// 初期化
ParticleData InitParticleData(uint index)
{
    ParticleData result;
    
    // インデックス(x,y)
    float2 seed = uint2(index % 256, index / 256) + uint2(-1, 1);

    // 座標
    float3 frontVec = normalize(vec);
    float3 rightVec = normalize(cross(frontVec, float3(0, 1, 0)));
    float3 upVec = normalize(cross(frontVec, rightVec));
    
    float sign = 0;
    
    // 一回真正面に伸ばす
    seed = RandomSeed(seed, index);
    sign = Random01(seed) > 0.5f ? +1 : -1;
    
    seed = RandomSeed(seed, index);
    result.pos = frontVec * Random01(seed) * length(vec) * sign;

    // 上下左右にずらす
    float dis = 2.f;
    seed = RandomSeed(seed, index);
    sign = Random01(seed) > 0.5f ? +1 : -1;
    
    seed = RandomSeed(seed, index);
    result.pos += rightVec * Random01(seed) * dis * sign;
    
    seed = RandomSeed(seed, index);
    sign = Random01(seed) > 0.5f ? +1 : -1;
    
    seed = RandomSeed(seed, index);
    result.pos += upVec * Random01(seed) * dis * sign;

    // ベクトル
    result.moveVec = normalize(float3(1, 1, 1));
        
    // 移動速度
    const float3 baseSpeed = float3(0.005f, -0.01f, 0.005f);
    const float3 rateSpeed = float3(0.01f, -0.005f, 0.01f);
    seed = RandomSeed(seed, index);
    result.moveAccel.x = baseSpeed.x - Random01(seed) * rateSpeed.x;
    seed = RandomSeed(seed, index);
    result.moveAccel.y = baseSpeed.y + Random01(seed) * rateSpeed.y;
    seed = RandomSeed(seed, index);
    result.moveAccel.z = baseSpeed.z - Random01(seed) * rateSpeed.z;
        
    // スケール
    seed = RandomSeed(seed, index);
    result.scale = 0.25f + Random01(seed) * 0.15f;
    
    // 輝度
    seed = RandomSeed(seed, index);
    result.shininess = 1.0f;
        
    // 色
    result.color = float4(0.53f, 0.f, 0.f, 0.f);
    result.color.rgb = rightVec;
    
    return result;
}