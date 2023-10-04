struct V2G
{
    float4 pos : SV_POSITION; // �V�X�e���p���_���W
    float2 scale : TEXCOORD0; // �X�P�[��
    float rot : TEXCOORD1; // ��]
    float shininess : TEXCOORD2;
    float4 color : COLOR; //�F
};

struct ParticleParam
{
    float3 pos;
    float2 scale;
    float rot;
    float shininess;
    float4 color;
};

static const uint MaxNum = 32;
struct Data
{
    ParticleParam param[MaxNum];
};
StructuredBuffer<Data> inputData : register(t1);

V2G main(uint SV_VertexID : SV_VertexID)
{
    V2G o = (V2G) 0;
    
    Data input = inputData[0];
    
    o.pos = float4(input.param[SV_VertexID].pos, 1);
    o.scale = input.param[SV_VertexID].scale;
    o.rot = input.param[SV_VertexID].rot;
    o.shininess = input.param[SV_VertexID].shininess;
    o.color = input.param[SV_VertexID].color;
    return o;
}