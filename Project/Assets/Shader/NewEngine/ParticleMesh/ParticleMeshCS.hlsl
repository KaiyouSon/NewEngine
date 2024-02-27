// �����_���̐�
float Random01(float2 seed)
{
    float result = frac(sin(dot(seed, float2(12.9898f, 78.223f))) * 43758.5453f);
    return result;
}

// �e�N�X�`���[�̖ʐ�
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
    //float2 offsetPos = 0;
    float2 seed = 0;
    
    for (uint i = 0; i < area; i++)
    {
        ParticleData result = outputData[i];
        
        // �V�[�g������
        seed = uint2(i % 256, i / 256);
        
        // ���W
        seed.x = Random01(seed);
        seed.y = Random01(seed);
        result.pos.x = Random01(seed) * size.x;
        seed.x = Random01(seed);
        seed.y = Random01(seed);
        result.pos.y = Random01(seed) * size.y;
        
        // �X�P�[��
        seed.x = Random01(seed);
        seed.y = Random01(seed);
        result.scale = 1.f + Random01(seed) * 1.f;
        
        // �P�x
        result.shininess = 1.f;
        
        // �F
        result.color = tex.Load(int3(result.pos.x, result.pos.y, 0));
        
        // ���S��
        float2 offsetPos = -size / 2.f;
        result.pos.x += offsetPos.x;
        result.pos.y += offsetPos.y;
        
        // �o�̓f�[�^����������
        outputData[i] = result;
    }
}
