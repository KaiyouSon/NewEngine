#include "ToneMapping.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 ToneCurve(float4 inputColor, float gain, float offset)
{
    // 入力ピクセルのRGB値を取得
    float3 rgb = inputColor.rgb;

    // トーンカーブの計算（各チャンネルに対してゲインとオフセットを適用）
    rgb = rgb * gain + offset;

    // 新しいRGB値を返す
    return float4(rgb, inputColor.a);
}

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);

    // トーンカーブの関数を呼び出し、結果を出力
    return ToneCurve(texColor, gain, offset);

    //return float4(texColor.rgb, 1);

}