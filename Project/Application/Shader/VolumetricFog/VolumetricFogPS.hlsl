#include "VolumetricFog.hlsli"

Texture3D<float4> tex : register(t0);
SamplerState smp : register(s0);

// レイからボックスに当たる最近接点と最遠点
float2 ClucRayToBoxCrossPoint(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// レイマーチング
float4 RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// 最小値を0に最大値を1にし値をlerpする
float3 MapValueTo01(float3 boundsMin, float3 boundsMax, float3 value);

float4 main(V2P i) : SV_TARGET
{
    float3 rayStart = cameraPos;
    float3 rayDir = normalize(i.wpos - cameraPos);
        
    // ボックスの最大最小値
    float3 boundsMin = objectPos + objectScale * float3(-0.5f, -0.5f, -0.5f);
    float3 boundsMax = objectPos + objectScale * float3(+0.5f, +0.5f, +0.5f);

    // カメラからの距離を計算
    float dis = distance(cameraPos, i.wpos);
    float disRate = smoothstep(fogClamp.x, fogClamp.y, dis);
    
    float4 resultColor = RayMarching(boundsMin, boundsMax, rayStart, rayDir) * disRate;
    
    resultColor = resultColor * fogColor * fogColorRate;
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
float4 RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir)
{
    float2 hitinfo = ClucRayToBoxCrossPoint(boundsMin, boundsMax, rayStart, rayDir);
    
    // 当たった回数を格納する
    uint hitCount = 0;
    
    float colorDensity = 0;
    
    // レイを飛ばして当たっていたら
    if (hitinfo.x <= hitinfo.y)
    {
        // レイの開始点(当たった点から)
        float3 rayPos = rayStart + rayDir * hitinfo.x;
        
        // ステップ分を進む
        //[unroll(1000)] 
        [loop]
        for (uint i = 0; i < stepCount; i++)
        {
            float3 uvw = MapValueTo01(boundsMin, boundsMax, rayPos);
            colorDensity += tex.Sample(smp, (uvw + offset)).r * stepLength * density;
        
            // 次のレイの座標を算出
            rayPos += rayDir * stepLength;
        }
    }
    
    return colorDensity;
}

// 最小値を0に最大値を1にし値をlerpする
float3 MapValueTo01(float3 boundsMin, float3 boundsMax, float3 value)
{
    float3 result = (value - boundsMin) / (boundsMax - boundsMin);
    return result;
}