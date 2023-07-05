// --- 頂点シェーダー入力構造体 ----------------------------- //
struct VSInputPos
{
    float4 pos : POSITION;
};

struct VSInputPosNormal
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
};

struct VSInputPosUv
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VSInputPosNormalUv
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSInputPosNormalUvBone
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 boneIndices : BONEINDICES; // ボーンの番号
    float4 boneWeights : BONEWEIGHTS; // ボーンのスキンウェイト
};

// --- 頂点シェーダー出力構造体 ----------------------------- //
struct VSOutputSvpos
{
    float4 svpos : SV_POSITION; // システム用頂点座標
};

struct VSOutputSvposUv
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD; // uv値
};

struct VSOutputSvposNormalUv
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float3 normal : NORMAL; // 法線ベクトル
    float2 uv : TEXCOORD; // uv値
};

struct VSOutputSvposPosNormalUv
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 worldPos : POSITION; // ワールド座標
    float3 normal : NORMAL; // 法線ベクトル
    float2 uv : TEXCOORD; // uv値
};