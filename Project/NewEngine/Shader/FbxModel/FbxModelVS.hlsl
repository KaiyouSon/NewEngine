#include "FbxModel.hlsli"

SkinOutput ComputeSkin(Appdata i)
{
    SkinOutput o = (SkinOutput) 0;
    
    uint iBone; // 計算するボーン番号
    float weight; // ボーンウェイト（重み）
    matrix skinMat; // スキニング行列
    
    // ボーン0
    iBone = i.boneIndices.x;
    weight = i.boneWeights.x;
    skinMat = skinningMat[iBone];
    o.pos += weight * mul(skinMat, i.pos);
    o.normal += weight * mul((float3x3) skinMat, i.normal);
    
    // ボーン1
    iBone = i.boneIndices.y;
    weight = i.boneWeights.y;
    skinMat = skinningMat[iBone];
    o.pos += weight * mul(skinMat, i.pos);
    o.normal += weight * mul((float3x3) skinMat, i.normal);
    
    // ボーン2
    iBone = i.boneIndices.z;
    weight = i.boneWeights.z;
    skinMat = skinningMat[iBone];
    o.pos += weight * mul(skinMat, i.pos);
    o.normal += weight * mul((float3x3) skinMat, i.normal);
    
    // ボーン3
    iBone = i.boneIndices.w;
    weight = i.boneWeights.w;
    skinMat = skinningMat[iBone];
    o.pos += weight * mul(skinMat, i.pos);
    o.normal += weight * mul((float3x3) skinMat, i.normal);
    
    return o;
}

V2P main(Appdata i)
{
    // スキニング計算
    SkinOutput skinned = ComputeSkin(i);
    
    // 法線にワールド行列によるスケーリング・回転を適用		
    float4 wnormal = normalize(mul(worldMat, float4(skinned.normal, 0)));
    float4 wpos = mul(worldMat, skinned.pos);
    float4 vertexPos = mul(mul(viewMat, worldMat), skinned.pos);

    // ピクセルシェーダーに渡す値
    V2P o = (V2P) 0;
    o.svpos = vertexPos;
    o.wpos = wpos;
    o.normal = wnormal.xyz;
    o.uv = i.uv;

    return o;
}
