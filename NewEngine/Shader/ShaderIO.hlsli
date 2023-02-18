// --- 頂点シェーダー入力構造体 -----------------------------  //
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

// --- 頂点シェーダー出力構造体 -----------------------------  //
struct VSOutputSvpos
{
    float4 svpos : SV_POSITION; // システム用頂点座標
};

struct VSOutputSvposUv
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD; // uv値
};

struct VSOutputSvposPosNormalUv
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 worldPos : POSITION; // ワールド座標
    float3 normal : NORMAL; // 法線ベクトル
    float2 uv : TEXCOORD; // uv値
};