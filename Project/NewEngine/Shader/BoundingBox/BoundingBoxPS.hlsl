#include "BoundingBox.hlsli"

Texture3D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

// レイからボックスに当たる最近接点と最遠点
float2 ClucRayToBoxCrossPoint(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// レイマーチング
uint RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir, Texture3D<float4> tex);

float4 main(V2P i) : SV_TARGET
{
    float3 rayStart = cameraPos;
    float3 rayDir = normalize(i.wpos - cameraPos);
        
    // ボックスの最大最小値
    float3 boundsMin = float3(-0.5f, -0.5f, -0.5f);
    float3 boundsMax = float3(+0.5f, +0.5f, +0.5f);

    uint hitCount = RayMarching(boundsMin, boundsMax, rayStart, rayDir, tex);
    
    float4 texColor = tex.Sample(smp, i.uvw);
    float4 resultColor = smoothstep(smoothingClamp.x, smoothingClamp.y, texColor * hitCount * fogColorRate);
    
    // レイマーチングで球を描画する処理
    //{
    //    float3 v = rayStart - float3(0, 0, 0);
    //    float dot1 = dot(v, rayDir);
    //    float dot2 = dot(v, v) - (0.5f * 0.5f);
    
    //    float discr = (dot1 * dot1) - dot2;
    //    if (dot1 > 0 && dot2 > 0)
    //    {
    //        resultColor = 0;
    //    }
    //    else if (discr < 0)
    //    {
    //        resultColor = 0;
    //    }
    //    else
    //    {
    //        resultColor = 1;
    //    }
    //}
    
    
    return resultColor;
}

// レイからボックスに当たる最近接点と最遠点
float2 ClucRayToBoxCrossPoint(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir)
{
    // 各軸（X、Y、Z）について交点の範囲を計算
    float3 t0 = (boundsMin - rayStart) / rayDir;
    float3 t1 = (boundsMax - rayStart) / rayDir;
    
    // 交点範囲をmin,maxに分ける
    float3 tmin = min(t0, t1);
    float3 tmax = max(t0, t1);
    
    // レイがボックス内に入り、最初に交差するパラメータTを算出（長さ）
    float nearestLength = max(max(tmin.x, tmin.y), tmin.z);
    
    // レイがボックスを通過し、最後に交差するパラメータTを算出（長さ）
    float farthestLength = min(min(tmax.x, tmax.y), tmax.z);
    
    // 最近接点と最遠点のそれぞれの距離を返す
    return float2(max(0, nearestLength), max(0, farthestLength));
}

// レイマーチング
uint RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir, Texture3D<float4> tex)
{
    float2 hitinfo = ClucRayToBoxCrossPoint(boundsMin, boundsMax, rayStart, rayDir);
    
    // レイの開始点
    float3 rayPos = rayStart;
    
    // 当たった回数を格納する
    uint hitCount = 0;
    
    for (uint step = 0; step < stepCount; step++)
    {
        //float disToSphere = length(rayPos - float3(0, 10, 0));
        //if (disToSphere <= 0.5f)
        //{
        hitCount++;
        //}
        
        // 次のレイの座標を算出
        rayPos += rayDir * stepLength;
    }
    
    return hitCount;
}