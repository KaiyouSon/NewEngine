struct V2G
{
    float4 pos : SV_POSITION; // システム用頂点座標
    float2 scale : TEXCOORD0; // スケール
    float rot : TEXCOORD1; // 回転
    float shininess : TEXCOORD2;
    float4 color : COLOR; //色
};

struct ParticleParam
{
    float3 pos;
    float2 scale;
    float rot;
    float shininess;
    float4 color;
};

static const uint MaxNum = 125000;
StructuredBuffer<ParticleParam> inputData : register(t1);

V2G main(uint SV_VertexID : SV_VertexID)
{
    V2G o = (V2G) 0;
    
    o.pos = float4(inputData[SV_VertexID].pos, 1);
    o.scale = inputData[SV_VertexID].scale;
    o.rot = inputData[SV_VertexID].rot;
    o.shininess = inputData[SV_VertexID].shininess;
    o.color = inputData[SV_VertexID].color;
    return o;
}
