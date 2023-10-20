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
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float scaleAccel;
    float shininess;
    float4 color;
};

StructuredBuffer<ParticleParam> inputData : register(t1);

V2G main(uint SV_VertexID : SV_VertexID)
{
    V2G o = (V2G) 0;
    
    o.pos = float4(inputData[SV_VertexID].pos, 1);
    o.scale = inputData[SV_VertexID].scale;
    o.rot = 0;
    o.shininess = inputData[SV_VertexID].shininess;
    o.color = inputData[SV_VertexID].color;
    return o;
}
