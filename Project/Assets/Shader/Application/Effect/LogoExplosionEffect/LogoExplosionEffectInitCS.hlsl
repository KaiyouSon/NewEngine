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

// テクスチャーの面積
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
    // area == 10wになっているから、[numthreads(1000, 1, 1)]だと1CSに100回for分回す
    
    // 100個のデータを処理するためのインデックスを計算
    uint dataPerThread = area / 1000;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;
    
    float2 offsetPos = 0;
    float2 seed = 0;
    
    for (uint i = startIndex; i < endIndex; i++)
    {
        ParticleData result = outputData[i];
        
        // インデックス(x,y)
        uint2 index = uint2((startIndex + i) % 256, (startIndex + i) / 256);
        
        // シート初期化
        seed.x = index.x;
        seed.y = index.y;
        
        // 座標
        seed = RandomSeed(seed, index);
        result.pos.x = Random01(seed) * size.x;
        seed = RandomSeed(seed, index);
        result.pos.y = Random01(seed) * size.y;

        // 移動ベクトル
        seed = RandomSeed(seed, index);
        result.moveVec.x = 1 - Random01(seed) * 2;
        seed = RandomSeed(seed, index);
        result.moveVec.y = 1 - Random01(seed) * 2;
        seed = RandomSeed(seed, index);
        result.moveVec.z = 1 - Random01(seed) * 2;
        
        // 移動速度
        const float baseSpeed = 4.f;
        const float rate = 2.f;
        seed = RandomSeed(seed, index);
        result.moveAccel.x = baseSpeed + Random01(seed) * rate;
        seed = RandomSeed(seed, index);
        result.moveAccel.y = baseSpeed + Random01(seed) * rate;
        seed = RandomSeed(seed, index);
        result.moveAccel.z = baseSpeed + Random01(seed) * rate;
        
        // スケール
        seed = RandomSeed(seed, index);
        result.scale = 1.f + Random01(seed) * 1.f;
        
        // 輝度
        seed = RandomSeed(seed, index);
        result.shininess = 1.f;
        
        // 色
        result.color = tex.Load(int3(result.pos.x, result.pos.y, 0));
        
        // タイマー
        seed = RandomSeed(seed, index);
        result.timer = Random01(seed) * 360;
        
        // 中心に
        float2 offsetPos = -size / 2.f;
        result.pos.x += offsetPos.x;
        result.pos.y += offsetPos.y;
        
        // 出力データを書き込む
        outputData[i] = result;
    }
}
