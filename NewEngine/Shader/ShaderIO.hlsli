// --- ���_�V�F�[�_�[���͍\���� -----------------------------  //
struct VSInputPosNormal
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
};

struct VSInputPosNormalUv
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

// --- ���_�V�F�[�_�[�o�͍\���� -----------------------------  //
struct VSOutputSvpos
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
};

struct VSOutputSvposUv
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float2 uv : TEXCOORD; // uv�l
};

struct VSOutputSvposPosNormalUv
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 worldPos : POSITION; // ���[���h���W
    float3 normal : NORMAL; // �@���x�N�g��
    float2 uv : TEXCOORD; // uv�l
};