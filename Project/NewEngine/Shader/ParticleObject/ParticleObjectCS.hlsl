// �����_���̐�
float Random01(float2 seed)
{
    float result = frac(sin(dot(seed, float2(12.9898f, 78.223f))) * 43758.5453f);
    return result;
}

// �e�N�X�`���[�̖ʐ�
cbuffer ConstantBufferParticleObject : register(b0)
{
    float maxParticleSize;
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
    for (uint i = 0; i < maxParticleSize; i++)
    {
        ParticleData result = outputData[i];
        result.color.rgb = modelData[0].pos;
        result.color.a = 1;
        
        // �o�̓f�[�^����������
        outputData[i] = result;
    }
}
