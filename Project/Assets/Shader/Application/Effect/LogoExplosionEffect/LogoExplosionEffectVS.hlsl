struct V2G
{
    float4 pos : SV_POSITION; // �V�X�e���p���_���W
    float2 scale : TEXCOORD0; // �X�P�[��
    float shininess : TEXCOORD1; // �P�x
    float4 color : COLOR; //�F
};

struct ParticleData
{
    float3 pos;
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float shininess;
    float4 color;
    float timer;
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
