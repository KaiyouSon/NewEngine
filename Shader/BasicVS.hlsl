#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//float4 wnormal = normalize(mul(worldMat, float4(normal, 0)));
	//float4 wpos = mul(worldMat, pos);

	//VSOutput output; // ピクセルシェーダーに渡す値
	//output.svpos = mul(mul(viewMat, worldMat), pos);
	//float3 eyeDir = normalize(cameraPos - wpos.xyz);

	//// 拡散反射光
	//float3 diffuse = dot(lightVec, wnormal.xyz) * diffuse;

	//float3 reflect = normalize(-lightVec + 2 * dot(lightVec, wnormal.xyz) * wnormal.xyz);

	//return output;

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos); // 座標に行列を乗算
	output.normal = normal;
	output.uv = uv;
	return output;
}