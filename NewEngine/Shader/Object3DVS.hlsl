#include "Object3DH.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	{
		// 法線にワールド行列によるスケーリング・回転を適用		
		float4 wnormal = normalize(mul(worldMat, float4(normal, 0)));
		float4 wpos = mul(worldMat, pos);

		VSOutput output; // ピクセルシェーダーに渡す値
		output.svpos = mul(mul(viewMat, worldMat), pos);
		output.worldPos = wpos;
		output.normal = wnormal.xyz;
		output.uv = uv;

		return output;
	}


	//{
	//	//// 右、下、左の方向を向いたライト
	//	//float3 lightDir = float3(1, -1, 1);
	//	//lightDir = normalize(lightDir);

	//	//// ライトの色
	//	//float3 lightColor = float3(1, 1, 1);

	//	float4 wnormal = normalize(mul(worldMat, float4(normal, 0)));
	//	float4 wpos = mul(worldMat, pos);

	//	// ピクセルシェーダーに渡す値
	//	VSOutput output;
	//	//output.svpos = mul(mat, pos); // 座標に行列を乗算
	//	output.svpos = mul(mul(viewMat, worldMat), pos);

	//	// Phone反射
	//	{
	//		// 環境反射光
	//		float3 tAmbient = ambient;

	//		// 拡散反射光
	//		//float3 tDiffuse = dot(-lightDir, normal) * diffuse;
	//		float3 tDiffuse = dot(lightVec, wnormal.xyz) * diffuse;

	//		// 視点座標
	//		const float3 eye = float3(0, 0, -20);
	//		// 光沢度
	//		const float shininess = 4.0f;
	//		// 頂点から視点へのベクトル
	//		//float3 eyeDir = normalize(eye - pos.xyz);
	//		float3 eyeDir = normalize(cameraPos - wpos.xyz);
	//		// 反射光ベクトル
	//		//float reflect = normalize(lightDir + 2 * dot(-lightDir, normal) * normal);
	//		float3 reflect = normalize(-lightVec + 2 * dot(lightVec, wnormal.xyz) * wnormal.xyz);
	//		// 鏡面反射光
	//		float tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

	//		output.color.rgb = (tAmbient + tDiffuse + tSpecular) * lightColor;
	//		output.color.a = alpha;
	//	}

	//	// Lambert反射
	//	{
	//		// Lambert反射の計算
	//		//output.color.rgb = dot(-lightDir, normal) * diffuse, lightColor;
	//		//output.color.a = alpha;
	//	}
	//	output.uv = uv;

	//	return output;
	//}

	//VSOutput output; // ピクセルシェーダーに渡す値
	//output.svpos = mul(mat, pos); // 座標に行列を乗算
	//output.normal = normal;
	//output.uv = uv;
	//return output;
}