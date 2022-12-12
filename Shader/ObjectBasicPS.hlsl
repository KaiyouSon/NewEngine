#include "ObjectBasic.hlsli"

Texture2D<float4> tex : register(t0);	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		// 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv);
	return input.color;

	//float3 light = normalize(float3(1,-1,1));	// 右下奥 向きのライト
	//float lightDiffuse = saturate(dot(-light, input.normal));
	//float3 shadeColor;
	//shadeColor = ambient;	// アンビエント項
	//shadeColor += diffuse * lightDiffuse;
	//float4 texColor = tex.Sample(smp, input.uv);

	//return float4(texColor.rgb * shadeColor, texColor.a * alpha);
}