#include "ToonRenderH.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
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