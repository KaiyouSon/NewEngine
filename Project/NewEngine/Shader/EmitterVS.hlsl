#include "Emitter.hlsli"

struct ArrayData
{
    float3 curPos;
    float2 curScale;
    float curRot;
    float curShininess;
    float4 curColor;
};

struct Data
{
    ArrayData data[2];
};

StructuredBuffer<Data> inputData : register(t1);

V2G main(uint SV_VertexID : SV_VertexID, Appdata i)
{
    V2G o = (V2G) 0;
    
    Data input = inputData[0];
    
    o.pos = float4(input.data[SV_VertexID].curPos, 1);
    o.scale = i.scale;
    o.rot = i.rot;
    o.shininess = i.shininess;
    o.color = i.color;
    return o;
}