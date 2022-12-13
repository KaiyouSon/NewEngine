#include "ObjectBasic.hlsli"

Texture2D<float4> tex : register(t0);	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		// 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャーマッピング
	float4 texColor = tex.Sample(smp, input.uv);

	// 光沢度
	const float shininess = 4.0f;
	// 頂点から視点へのベクトル
	float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
	// ライトに向かうベクトルと法線の内積
	float3 dotLightNormal = dot(lightVec, input.normal);
	// 反射光ベクトル
	float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);

	// 環境反射光
	float3 tAmbient = ambient;
	// 拡散反射光
	float3 tDiffuse = dotLightNormal * diffuse;
	// 鏡面反射光
	float tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

	// シェーディングによる色
	float4 shaderColor;
	shaderColor.rgb = (tAmbient + tDiffuse + tSpecular) * lightColor;
	shaderColor.a = alpha;

	return shaderColor * texColor;



	//float4 texColor = tex.Sample(smp, input.uv);
	//return input.color * texColor;

	//float3 light = normalize(float3(1,-1,1));	// 右下奥 向きのライト
	//float lightDiffuse = saturate(dot(-light, input.normal));
	//float3 shadeColor;
	//shadeColor = ambient;	// アンビエント項
	//shadeColor += diffuse * lightDiffuse;
	//float4 texColor = tex.Sample(smp, input.uv);

	//return float4(texColor.rgb * shadeColor, texColor.a * alpha);
}