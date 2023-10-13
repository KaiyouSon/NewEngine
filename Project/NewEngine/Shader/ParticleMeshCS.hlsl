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
    
    for (uint i = 0; i < area / 10; i++)
    {
        ParticleData result = outputData[i];
        result.pos.x = -size.x / 2.f + offsetPos.x * 10.f;
        result.pos.y = +size.y / 2.f - offsetPos.y * 10.f;
        result.scale = 50.f;
        result.color = tex.Load(int3(offsetPos.x, offsetPos.y, 0));
        result.shininess = 2.f;

        offsetPos.x += 5;
        if (offsetPos.x >= size.x)
        {
            offsetPos.x = 0;
            offsetPos.y += 5;
        }
        
        // 出力データを書き込む
        outputData[i] = result;
    }
}
