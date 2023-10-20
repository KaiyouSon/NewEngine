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

[numthreads(50, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint dataPerThread = max / 50;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;
    
    for (uint i = startIndex; i < endIndex; i++)
    {
        ParticleData result = outputData[i];
        
        if (result.scale.x <= 0)
        {
            result.scale = 0;
        }

        result.pos += result.moveVec * result.moveAccel;
        
        result.color.a += result.alphaAccel;
        if (result.color.a >= 1)
        {
            result.color.a = 1;
            result.scale -= result.scaleAccel;
        }
        
        outputData[i] = result;
    }
}