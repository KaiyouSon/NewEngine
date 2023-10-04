#include "Emitter.hlsli"

struct InputData
{
    float3 curPos;
    float2 curScale;
    float curRot;
    float curShininess;
    float4 curColor;
};

StructuredBuffer<InputData> inputData : register(t1);

V2G main(Appdata i)
{
    V2G o = (V2G) 0;
    
    InputData input = inputData[0];
    
    o.pos = float4(input.curPos, 1);
    o.scale = i.scale;
    o.rot = i.rot;
    o.shininess = i.shininess;
    o.color = i.color;
    return o;
}