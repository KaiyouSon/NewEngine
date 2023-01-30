#include "Object3DH.hlsli"

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
		if (directionalLights[i].isActive == true)
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
		}
	}

	// 点光源
	for (int i = 0; i < PointLightNum; i++)
	{
		if (pointLights[i].isActive == true)
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

	// スポットライト
	for (int i = 0; i < spotLightNum; i++)
	{
		if (spotLights[i].isActive == true)
		{
			// ライトへの方向ベクトル
			float3 lightv = spotLights[i].pos - input.worldPos.xyz;
			// ベクトルの長さ
			float d = length(lightv);
			// 正規化
			lightv = normalize(lightv);

			// 距離減衰係数
			float atten = saturate(1.0f /
				(spotLights[i].atten.x +
					spotLights[i].atten.y * d +
					spotLights[i].atten.z * d * d));

			// 角度減衰
			float cos = dot(lightv, spotLights[i].vec);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は１倍、減衰終了角度の外側は０倍の輝度
			float angleAtten = smoothstep(spotLights[i].factorAngleCos.y, spotLights[i].factorAngleCos.x, cos);
			// 角度減衰を乗算
			atten *= angleAtten;

			// ライト方向に向かうベクトルと法線の内積
			float3 dotLightNormal = dot(lightv, input.normal);

			// 反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotLightNormal * input.normal);

			// 拡散反射光
			float3 tDiffuse = dotLightNormal * diffuse;
			// 鏡面反射光
			float3 tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

			// 全て加算する
			shaderColor.rgb += atten * (tDiffuse + tSpecular) * spotLights[i].color;
		}
	}

	// 丸影
	for (int i = 0; i < circleShadowNum; i++)
	{
		if (circleShadows[i].isActive == true)
		{
			// オブジェクト表面からキャスターへのベクトル
			float3 casterV = circleShadows[i].pos - input.worldPos.xyz;

			// 投影方向での距離
			float d = dot(casterV, circleShadows[i].vec);

			// 距離減衰係数
			float atten = saturate(1.0f /
				(circleShadows[i].atten.x +
					circleShadows[i].atten.y * d +
					circleShadows[i].atten.z * d * d));
			// 距離がマイナスなら０にする
			atten *= step(0, d);

			// 仮想ライトの座標
			float3 lightPos = circleShadows[i].pos + circleShadows[i].vec * circleShadows[i].disCasterLight;

			// オブジェクト表面からライトへのベクトル
			float3 lightV = normalize(lightPos - input.worldPos.xyz);

			// 角度計算
			float cos = dot(lightV, circleShadows[i].vec);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は１倍、減衰終了角度の外側は０倍の輝度
			float angleAtten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// 角度減衰を乗算
			atten *= angleAtten;

			// 全て減算する
			shaderColor.rgb -= atten;
		}
	}

	if (isActiveFog == true)
	{
		// カメラの座標と頂点の距離
		float dis = distance(input.worldPos.xyz, cameraPos);
		float rate = smoothstep(fogNearDis, fogFarDis, dis);

		// フォグの色
		float4 tFogColor = fogColor * rate;
		return shaderColor * texColor * color + tFogColor;
	}
	else
	{
		return shaderColor * texColor * color;
	}
}