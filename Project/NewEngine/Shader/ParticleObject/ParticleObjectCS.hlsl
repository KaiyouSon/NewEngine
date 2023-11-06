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
    uint meshParitcleSize;
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
    uint meshNum = vertexSize / 3;
    
    for (uint i = 0; i < meshNum; i++)
    {
        float3 p0 = modelData[pointIndexOffset].pos;
        float3 p2 = modelData[pointIndexOffset + 1].pos;
        float3 p1 = modelData[pointIndexOffset + 2].pos;
    
        float2 seed = float2(10, 1);
        for (uint j = 0; j < meshParitcleSize / 2; j++)
        {
            uint index = i * meshParitcleSize / 2 + j;
            
            ParticleData result = outputData[index];
            float3 centroidPos = 0;
        
            if (j < 3)
            {
                result.pos = modelData[pointIndexOffset + j].pos;
                result.color.rgb = float4(0, 0, 1, 1);
                result.scale = 0.25f;

            }
            else
            {
                seed = RandomSeed(seed, index);
                float a = Random01(seed);
                seed = RandomSeed(seed, index);
                float b = (1 - a) * Random01(seed);
                float c = 1 - a - b;
    
                centroidPos.x = a * p0.x + b * p1.x + c * p2.x;
                centroidPos.y = a * p0.y + b * p1.y + c * p2.y;
                centroidPos.z = a * p0.z + b * p1.z + c * p2.z;
                result.pos = centroidPos;
                result.color.rgb = float4(1, 0, 0, 1);
                
                if (pointIndexOffset == 18)
                {
                    result.color.rgb = float4(0, 1, 0, 1);
                }
                
                result.scale = 0.05f;
            }
       
            result.shininess = 1;
            result.color.a = 1;
        
            // 出力データを書き込む
            outputData[index] = result;
            
        }
        
        pointIndexOffset += 3;
    }
}
