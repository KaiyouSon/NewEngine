#include "ObjectBasic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	float3 lightDir = float3(1, -1, 1);
	lightDir = normalize(lightDir);

	float3 lightColor = float3(1, 1, 1);

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos); // 座標に行列を乗算

	// Lambert反射を計算
	output.color.rgb = dot(-lightDir, normal) * diffuse * lightColor;
	output.color.a = alpha;
	//output.normal = normal;
	output.uv = uv;
	return output;

	//VSOutput output; // ピクセルシェーダーに渡す値
	//output.svpos = mul(mat, pos); // 座標に行列を乗算
	//output.normal = normal;
	//output.uv = uv;
	//return output;
}