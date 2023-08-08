#include "Object3D.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> dissolveTex : register(t1); // 0番スロットに設定されたテクスチャ
Texture2D<float4> shadowMapTex : register(t2); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

PSOutput main(V2P i)// : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
	// テクスチャーマッピング
    float4 texColor = tex.Sample(smp, newUV);
    float4 mask = dissolveTex.Sample(smp, newUV);
    
    float maskIntensity = smoothstep(0.1, 0.25, (0.5f + mask.r) - dissolve);
    clip(maskIntensity - 0.01f);
    
	// 光沢度
    const float shininess = 4.0f;

    // マテリアル
    Material material = { ambient, diffuse, specular };
    
    // シェーダーカラー
    float4 shaderColor = 0;
    
    float4 adsColor = float4(0, 0, 0, 1);
    if (isActiveDirLight == true)
    {
        // ライトに向かうベクトルと法線の内積
        float dotLightNormal = dot(dirLightVec, i.normal);
        
        // アンビエント
        float3 ambient = /*texColor.rgb * 0.1f **/material.ambient.rgb;
     
        // ディフューズ
        float intensity = saturate(dot(normalize(i.normal), dirLightVec));
        float4 diffuse = intensity * dirLightColor * float4(material.diffuse.rgb, 1);
    
        // スペキュラー
        float3 eyeDir = normalize(cameraPos - i.wpos.xyz); // 頂点から視点へのベクトル
        float3 reflectDir = -dirLightVec + 2 * i.normal * dot(i.normal, dirLightVec);
        float3 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * material.specular.rgb;
    
        adsColor.rgb = ambient + diffuse + specular * dirLightColor;
    }
    
    float shadow = 1.0f;
    //シャドウマップのZ値を参照
    float w = 1.0f / i.spos.w;
    float2 shadowTexUV;
    shadowTexUV.x = (1.0f + i.spos.x * w) * 0.5f;
    shadowTexUV.y = (1.0f - i.spos.y * w) * 0.5f;
		//uv座標で0～1なら影判定をする
    if (shadowTexUV.x >= 0 && shadowTexUV.x <= 1.0f && shadowTexUV.y >= 0 && shadowTexUV.y <= 1.0f)
    {
        if (shadowMapTex.Sample(smp, shadowTexUV).x + 0.0005f < i.spos.z * w)
        {
            shadow *= 0.5f;
        }
    }
    
    float4 resultColor = (adsColor * texColor * color);
    resultColor.rgb *= shadow;
    
    PSOutput output;
    output.target0 = resultColor * maskIntensity + dissolveColor * colorPower * (1 - maskIntensity);
    //output.target0 = float4(shadow, shadow, shadow, 1);
    output.target1 = float4(1 - resultColor.rgb, color.a);
    return output;
    
    //return shaderColor * texColor * color;
}