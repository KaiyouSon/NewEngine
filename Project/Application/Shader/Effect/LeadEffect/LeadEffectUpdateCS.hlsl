struct ParticleData
{
    float3 pos;
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float scaleAccel;
    float shininess;
    float4 color;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

struct LeadEffect
{
    uint timer;
    uint maxTimer;
    float timerRate;
    uint startIndex;
    uint endIndex;
};
RWStructuredBuffer<LeadEffect> leadEffect : register(u1);

// パーティクルの最大数
cbuffer ConstantBufferMaxParticleData : register(b0)
{
    uint max;
}

// パーティクルのベースベクトル
cbuffer ConstantBufferVec : register(b1)
{
    float3 vec;
}

// 疑似乱数
float Random01(float2 seed);

// 疑似乱数シード
float2 RandomSeed(float2 seed, uint2 index);

// 任意軸回転
float3 AnyAxisRotation(float3 vec, float3 axis, float angle);

// 初期化
ParticleData InitParticleData(uint index);

[numthreads(1000, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x == 0)
    {
        leadEffect[0].timer++;
        if (leadEffect[0].timer >= leadEffect[0].maxTimer)
        {
            // 初期化
            for (uint i = 0; i < 3; i++)
            {
                outputData[leadEffect[0].endIndex] = InitParticleData(leadEffect[0].endIndex);
                leadEffect[0].endIndex++;
            }
            
            leadEffect[0].timer = 0;
            if (leadEffect[0].endIndex >= max)
            {
                leadEffect[0].endIndex = 0;
            }
        }
    }
    
    
    // area == 10wになっているから、[numthreads(1000, 1, 1)]だと1CSに10回for分回す
    
    // 100個のデータを処理するためのインデックスを計算
    uint dataPerThread = max / 1000;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;

    for (uint i = startIndex; i < endIndex; i++)
    {
        if (i >= leadEffect[0].endIndex)
        {
            return;
        }
        
        ParticleData result = outputData[i];
        
        // スケールが0の場合
        if (result.scale.x <= 0)
        {
            result.scale = 0;
            continue;
        }
        
        // 座標
        result.pos += normalize(result.moveVec) * result.moveAccel;
        
        result.scale -= 0.0001f;
        
        // 出力データを書き込む
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

// 任意軸回転
float3 AnyAxisRotation(float3 vec, float3 axis, float angle)
{
    float radian = angle * (3.1415926f / 180.f);
    float hlafRadian = radian / 2;
    float4 q = float4(normalize(axis).xyz * sin(hlafRadian), cos(hlafRadian));
    
    float3 qVec = q.xyz;
    float3 t = 2.0f * cross(qVec, vec);
    
    float3 result = vec + q.w * t + cross(qVec, t);
    return result;
}

// 初期化
ParticleData InitParticleData(uint index)
{
    ParticleData result;
    
    // インデックス(x,y)
    float2 seed = uint2(index % 256, index / 256) + uint2(-1, 1);

    // 座標
    result.pos = 0;

    // 右ベクトルを求める
    float3 rightVec = cross(vec, float3(0, 1, 0));
        
    // ベースベクトルの右ベクトル基準に上下に回転
    seed = RandomSeed(seed, index);
    float angle1 = Random01(seed) * 30 - 15;
    float3 roted1 = AnyAxisRotation(vec, rightVec, angle1);

    // ベースベクトルを基準にもう一回回転
    seed = RandomSeed(seed, index);
    float angle2 = Random01(seed) * 360;
    result.moveVec = AnyAxisRotation(roted1.xyz, vec, angle2).xyz;
        
    // 移動速度
    const float rate = 0.005f;
    const float baseSpeed = 0.005f;
    seed = RandomSeed(seed, index);
    result.moveAccel.x = baseSpeed + Random01(seed) * rate;
    seed = RandomSeed(seed, index);
    result.moveAccel.y = baseSpeed + Random01(seed) * rate;
    seed = RandomSeed(seed, index);
    result.moveAccel.z = baseSpeed + Random01(seed) * rate;
        
    // スケール
    seed = RandomSeed(seed, index);
    result.scale = 0.15f + Random01(seed) * 0.25f;

    // スケールの変化量
    const float baseScaleAccel = result.scale > 0.25f ? 0.0005f : 0.0001f;
    seed = RandomSeed(seed, index);
    result.scaleAccel = baseScaleAccel + Random01(seed) * 0.0005f;
    
    // 輝度
    seed = RandomSeed(seed, index);
    result.shininess = 0.75f + Random01(seed) * 0.75f;
        
    // 色
    result.color = float4(0.76f, 0.75f, 0.47f, 1.f);
    
    return result;
}