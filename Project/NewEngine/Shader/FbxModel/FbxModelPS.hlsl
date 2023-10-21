#include "FbxModel.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(V2P i) : SV_TARGET
{
	// テクスチャーマッピング
    float4 texColor = tex.Sample(smp, i.uv);
    return texColor * color;
}
