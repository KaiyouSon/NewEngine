#include "GPUEmitter.hlsli"

struct ArrayData
{
    float3 pos;
    float2 scale;
    float rot;
    float shininess;
    float4 color;
};

struct Data
{
    ArrayData data[2];
};

StructuredBuffer<Data> inputData : register(t1);

V2G main(uint SV_VertexID : SV_VertexID)
{
    V2G o = (V2G) 0;
    
    Data input = inputData[0];
    
    o.pos = float4(input.data[SV_VertexID].pos, 1);
    o.scale = input.data[SV_VertexID].scale;
    o.rot = input.data[SV_VertexID].rot;
    o.shininess = input.data[SV_VertexID].shininess;
    o.color = input.data[SV_VertexID].color;
    return o;
}
