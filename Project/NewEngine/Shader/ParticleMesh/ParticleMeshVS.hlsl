#include "ParticleMesh.hlsli"

struct ParticleData
{
    float3 pos;
    float2 scale;
    float shininess;
    float4 color;
};

StructuredBuffer<ParticleData> inputData : register(t1);

V2G main(uint SV_VertexID : SV_VertexID)
{
    V2G o = (V2G) 0;
    
    ParticleData input = inputData[SV_VertexID];
    
    o.pos = float4(input.pos, 1);
    o.scale = input.scale;
    o.shininess = input.shininess;
    o.color = input.color;
    return o;
}
