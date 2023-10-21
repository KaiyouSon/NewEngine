// ランダムの数
float Random01(float2 seed)
{
    float result = frac(sin(dot(seed, float2(12.9898f, 78.223f))) * 43758.5453f);
    return result;
}

// テクスチャーの面積
cbuffer ConstantBufferTextureSizeData : register(b0)
{
    float2 size;
    float area;
}

struct ParticleData
{
    float3 pos;
    float2 scale;
    float shininess;
    float4 color;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

Texture2D<float4> tex : register(t0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    int2 offsetPos = 0;
    
    for (uint i = 0; i < area; i++)
    {
        ParticleData result = outputData[i];
        
        while (true)
        {
            float2 seed = 0;
            // シート設定
            seed.x = -size.x / 2.f + offsetPos.x;
            seed.y = +size.y / 2.f - offsetPos.y;
            result.pos.x = Random01(seed) * size.x;
            // シート設定
            seed.x = +size.y / 2.f + offsetPos.x;
            seed.y = -size.x / 2.f - offsetPos.y;
            result.pos.y = Random01(seed) * size.y;

            result.color = tex.Load(int3(result.pos.x, result.pos.y, 0));
            if (result.color.a != 0)
            {
                break;
            }
        }
        float2 seed = 0;
        seed.x = offsetPos.x * offsetPos.y;
        seed.y = result.color.a * size.x / offsetPos.y;
        result.scale = 1.f + Random01(seed) * 1.f;
        result.shininess = 2.f;

        offsetPos.x++;
        if (offsetPos.x >= size.x)
        {
            offsetPos.x = 0;
            offsetPos.y++;
        }
        
        // 出力データを書き込む
        outputData[i] = result;
    }
}
