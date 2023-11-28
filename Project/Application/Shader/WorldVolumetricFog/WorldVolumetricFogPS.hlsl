#include "WorldVolumetricFog.hlsli"

Texture3D<float4> tex : register(t0);
SamplerState smp : register(s0);

// レイからボックスに当たる最近接点と最遠点
float2 ClucRayToBoxCrossPoint(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// レイマーチング
float4 RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// 最小値を0に最大値を1にし値をlerpする
float3 MapValueTo01(float3 boundsMin, float3 boundsMax, float3 value);

uint CheckWithInBox(float3 boundsMin, float3 boundsMax, float3 value)
{
    uint result = (
        value.x >= boundsMin.x && value.x <= boundsMax.x &&
        value.y >= boundsMin.y && value.y <= boundsMax.y &&
        value.z >= boundsMin.z && value.z <= boundsMax.z) ? 1 : 0;

    return result;
}

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
    
    float4 rayMarchingColor = RayMarching(boundsMin, boundsMax, localRayStart, localRayDir);
    float4 resultColor = rayMarchingColor;
    
    resultColor = (resultColor) * fogColor * fogColorRate;

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

    // レイの開始点(当たった点から)
    float3 rayPos = rayStart + rayDir * hitinfo.x;
    
    float alpha = 0;

    float3 scale = 1.0f / (boundsMax - boundsMin);
    uint hitCount = 0;
    
    float3 lightting = 1.0f;
    
    // ステップ分を進む
    [loop]
    for (uint i = 0; i < stepCount; i++)
    {
        if (CheckWithInBox(boundsMin, boundsMax, rayPos) == true)
        {
            float3 smpPoint = rayPos * scale + float3(0.5f, 0.5f, 0.5f);
            float color = tex.Sample(smp, (smpPoint + offset) * tiling).r * stepLength * density;
            
            colorDensity += color; // * disRate;
            
            //uint index = 0;
            
            //for (index = 0; index < 3; index++)
            //{
            //    if (pointLight[index].isActive == true)
            //    {
            //        // ライトヘのベクトル
            //        float3 lightVec = normalize(pointLight[index].pos - objectPos - rayPos);
            //        float d = distance(pointLight[index].pos - objectPos, rayPos);
            
            //        float s = d / pointLight[index].radius;
            //        if (s >= 1.0)
            //        {
            //            continue;
            //        }
            
            //        float s2 = s * s;
            
            //        float atten = pointLight[index].decay * ((1 - s2) * (1 - s2));
                
            //        lightting.rgb += atten * color *
            //                pointLight[index].color.rgb * pointLight[index].colorRate.rgb;
            //    }
            //}
            
            //// スポットライトの計算
            //for (index = 0; index < spotLightSize; index++)
            //{
            //    if (spotLight[index].isActive == true)
            //    {
            //        // ライトヘのベクトル
            //        float3 lightVec = normalize(spotLight[index].pos - objectPos - rayPos);
            //        float d = distance(spotLight[index].pos - objectPos, rayPos);
                    
            //        float s = d / spotLight[index].radius;
            //        if (s >= 1.0)
            //        {
            //            continue;
            //        }
            
            //        float s2 = s * s;
            
            //        float cosAngle = dot(lightVec, spotLight[index].vec);
            //        float falloffFactor = saturate((cosAngle - spotLight[index].cosAngle.y) / (spotLight[index].cosAngle.x - spotLight[index].cosAngle.y));
            
            //        float atten = spotLight[index].decay * ((1 - s2) * (1 - s2));
            //        atten *= falloffFactor;
            
            //        lightting += atten * color * 
            //                spotLight[index].color.rgb * spotLight[index].colorRate.rgb;
            //    }
            //}
        }
        
        // 次のレイの座標を算出
        rayPos += rayDir * stepLength;
    }
    
    return float4(colorDensity.xxx * lightting.rgb, colorDensity);
}

// 最小値を0に最大値を1にし値をlerpする
float3 MapValueTo01(float3 boundsMin, float3 boundsMax, float3 value)
{
    float3 result = (value - boundsMin) / (boundsMax - boundsMin);
    return result;
}