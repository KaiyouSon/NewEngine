#include "Object3D.hlsli"
#include "ShaderIO.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

PSOutput main(V2P i)// : SV_TARGET
{
    float2 newUV = (i.uv + offset) * tiling;
    
	// テクスチャーマッピング
    float4 texColor = tex.Sample(smp, newUV);
    
	// 光沢度
    const float shininess = 4.0f;

    // マテリアル
    Material material = { ambient, diffuse, specular };
    
    // シェーダーカラー
    float4 shaderColor = 0;
    
    // フォーン
    
    float4 adsColor = float4(0, 0, 0, 1);
    if (isActiveDirLight == true)
    {
        // アンビエント
        float4 ambient = texColor * 0.1f * dirLightColor * float4(material.ambient.rgb, 1);
     
        // ディフューズ
        float intensity = saturate(dot(normalize(i.normal), dirLightVec));
        float4 diffuse = intensity * dirLightColor * float4(material.diffuse.rgb, 1);;
    
        // スペキュラー
        float3 eyeDir = normalize(cameraPos - i.svpos.xyz); // 頂点から視点へのベクトル
        float3 reflectDir = -dirLightVec + 2 * i.normal * dot(i.normal, dirLightVec);
        float4 specular = pow(saturate(dot(reflectDir, eyeDir)), shininess) * dirLightColor * float4(material.specular.rgb, 1);
    
        adsColor = ambient + diffuse + specular;
        adsColor.a = 1;
    }

    float4 resultColor = adsColor * texColor * color;
    
    PSOutput output;
    output.target0 = resultColor;
    output.target1 = float4(1 - resultColor.rgb, color.a);
    return output;
    
    //return shaderColor * texColor * color;
}