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
    
    // レイをOBBのローカル座標系に変換
    matrix invRotateMat = transpose(rotateMat);
    float3 localRayStart = mul(invRotateMat, float4(rayStart - objectPos, 1)).xyz;
    float3 localRayDir = mul(invRotateMat, float4(rayDir, 1)).xyz;
    
    // ボックスの最大最小値
    float3 boundsMin = objectScale * -0.5f;
    float3 boundsMax = objectScale * +0.5f;

    // カメラからの距離を計算
    float dis = distance(cameraPos, i.wpos);
    float disRate = smoothstep(fogClamp.x, fogClamp.y, dis);
    
    float4 resultColor = RayMarching(boundsMin, boundsMax, localRayStart, localRayDir) /* * disRate*/;
    
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
    
    float colorDensity = 0.0f;
    float alpha = 0;
    
    // レイを飛ばして当たっていたら
    if (hitinfo.x <= hitinfo.y)
    {
        // レイの開始点(当たった点から)
        float3 rayPos = rayStart + rayDir * hitinfo.x;

        // ステップ分を進む
        [loop]
        for (uint i = 0; i < stepCount; i++)
        {
            float3 uvw = MapValueTo01(boundsMin, boundsMax, rayPos);
            colorDensity += tex.Sample(smp, (uvw + offset)).r * stepLength * density;
            
            float3 clamped = clamp(uvw, 0, 1);
            float dis = distance(clamped, float3(0.5f, 0.5f, 0.5f));
            alpha += (1 - smoothstep(0.05, 0.5f, dis)) * colorDensity;
            
            // 次のレイの座標を算出
            rayPos += rayDir * stepLength;
        }
    }
    return float4(colorDensity.xxx, alpha);
}

// 最小値を0に最大値を1にし値をlerpする
float3 MapValueTo01(float3 boundsMin, float3 boundsMax, float3 value)
{
    float3 result = (value - boundsMin) / (boundsMax - boundsMin);
    return result;
}