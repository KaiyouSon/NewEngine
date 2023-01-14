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

	// 環境反射光
	float3 tAmbient = ambient;

	float4 shaderColor = float4(float3(1,1,1) * tAmbient, alpha);

	// 平行光源
	for (int i = 0; i < DirectionalLightNum; i++)
	{
		if (directionalLights[i].isActive)
		{
			// ライトに向かうベクトルと法線の内積
			float3 dotLightNormal = dot(directionalLights[i].lightVec, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-directionalLights[i].lightVec + 2 * dotLightNormal * input.normal);

			// 拡散反射光
			float3 tDiffuse = dotLightNormal * diffuse;
			// 鏡面反射光
			float tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

			// 全て加算する
			shaderColor.rgb += (tDiffuse + tSpecular) * directionalLights[i].color;

			// シェーディングによる色
			//shaderColor.rgb += (tAmbient + tDiffuse + tSpecular) * color;
			//shaderColor.a = alpha;
		}
	}

	// 点光源
	for (int i = 0; i < PointLightNum; i++)
	{
		if (pointLights[i].isActive)
		{
			// ライトへの方向ベクトル
			float3 lightv = pointLights[i].pos - input.worldPos.xyz;
			// ベクトルの長さ
			float d = length(lightv);
			// 正規化
			lightv = normalize(lightv);

			// 距離減衰係数
			float atten = 1.0f /
				(pointLights[i].atten.x +
				pointLights[i].atten.y * d +
				pointLights[i].atten.z * d * d);

			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);

			// 反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 tDiffuse = dotlightnormal * diffuse;
			// 鏡面反射光
			float3 tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

			// 全て加算する
			shaderColor.rgb += atten * (tDiffuse + tSpecular) * pointLights[i].color;
		}
	}

	return shaderColor * texColor * color;
}