#include "ToonRendering.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutputSvposPosNormalUv vsOutput) : SV_TARGET
{
	// テクスチャーマッピング
    float4 texColor = tex.Sample(smp, vsOutput.uv);

	// 光沢度
    const float shininess = 4.0f;

	// 頂点から視点へのベクトル
    float3 eyeDir = normalize(cameraPos - vsOutput.worldPos.xyz);

    // マテリアル
    Material material = { ambient, diffuse, specular };
    
    // シェーダーカラー
    float4 shaderColor = float4(float3(1, 1, 1) * material.ambient, alpha);

    // 影の中間値
    const float middelValue = -0.35;

	// 平行光源
    for (int i = 0; i < directionalLightNum; i++)
    {
        // ライトに向かうベクトルと法線の内積
        float3 lightNormal = dot(directionalLights[i].vec, vsOutput.normal);
        lightNormal = smoothstep(middelValue, middelValue, lightNormal) ? 1.f : 0.1f;
        
        float3 color = CalculateDirectionalLight(
            directionalLights[i], material, vsOutput.normal, lightNormal, eyeDir, shininess);
        shaderColor.rgb += color;
    }

    // 光源計算用データ
    LightCalculateData lightCalculateData;
    
	// 点光源
    for (int i = 0; i < pointLightNum; i++)
    {
        // 頂点のワールド座標
        lightCalculateData.vertexPos = vsOutput.worldPos.xyz;
        // 頂点法線
        lightCalculateData.vertexNormal = vsOutput.normal;
        // 頂点のワールド座標からライトに向かうベクトル
        lightCalculateData.lightVec = normalize(pointLights[i].pos - vsOutput.worldPos.xyz);
        // ライトに向かうベクトルと法線の内積
        float3 lightNormal = dot(lightCalculateData.lightVec, vsOutput.normal);
        lightCalculateData.lightNormal = smoothstep(middelValue, middelValue, lightNormal) ? 1.f : 0.1f;
        
        float3 color = CalculatePointLight(
            pointLights[i], material, lightCalculateData, eyeDir, shininess);
        shaderColor.rgb += color;
    }
  
	// スポットライト
    for (int i = 0; i < spotLightNum; i++)
    {
        // 頂点のワールド座標
        lightCalculateData.vertexPos = vsOutput.worldPos.xyz;
        // 頂点法線
        lightCalculateData.vertexNormal = vsOutput.normal;
        // 頂点のワールド座標からライトに向かうベクトル
        lightCalculateData.lightVec = normalize(spotLights[i].pos - vsOutput.worldPos.xyz);
        // ライトに向かうベクトルと法線の内積
        float3 lightNormal = dot(lightCalculateData.lightVec, vsOutput.normal);
        lightCalculateData.lightNormal = smoothstep(middelValue, middelValue, lightNormal) ? 1.f : 0.1f;
        
        float3 color = CalculateSpotLight(
            spotLights[i], material, lightCalculateData, eyeDir, shininess);
        shaderColor.rgb += color;
    }

	// 丸影
    for (int i = 0; i < circleShadowNum; i++)
    {
        float3 color = CalculateCircleShadow(circleShadows[i], vsOutput.worldPos.xyz);
        shaderColor.rgb += color;
    }

    return shaderColor * texColor * color;
}
