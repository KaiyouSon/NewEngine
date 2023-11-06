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
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint pointIndexOffset = 0;
    uint meshMaxSize = vertexSize / 3;
    float2 seed = 0;
    for (uint meshIndex = 0; meshIndex < meshMaxSize; meshIndex++)
    {
        // 三角形の各頂点を算出する
        float3 p0 = modelData[pointIndexOffset].pos;
        float3 p1 = modelData[pointIndexOffset + 1].pos;
        float3 p2 = modelData[pointIndexOffset + 2].pos;
        
        uint start = (meshIndex) * meshParticleSize;
        uint end = (meshIndex + 1) * meshParticleSize;
        
         // パーティクルの設定
        for (uint i = start; i < end; i++)
        {
            if (i > maxParticleSize)
            {
                return;
            }
            
            ParticleData result = outputData[i];
            seed = float2(i, i + 2);

            // 頂点(最初の三点を頂点のところに配置する)
            if (i < start + 3)
            {
                uint vertexIndex = pointIndexOffset + i - start;
                result.pos = modelData[vertexIndex].pos;
                result.color = float4(0, 0, 1, 1);
                result.scale = 0.05f;
            }
            else
            {
                float a = Random01(seed / 10000);
                float b = (1 - a) * Random01(seed / 10000 + 1);
                float c = 1 - a - b;
    
                // 重心座標系
                float3 centroidPos = a * p0 + b * p1 + c * p2;
                
                result.pos = centroidPos;
                result.color = float4(1, 0, 0, 1);
                result.scale = 0.01f;
            }
            
            result.shininess = 1;
            
            outputData[i] = result;
        }
        
        
        pointIndexOffset += 3;
    }
}
