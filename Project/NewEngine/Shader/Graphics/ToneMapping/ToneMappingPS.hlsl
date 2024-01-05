#include "ToneMapping.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float ToneCurve(float input, float gain, float offset)
{
    float output = input * gain + offset;
    return output;
}

float4 main(V2P i) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, i.uv);

    texColor.r = ToneCurve(texColor.r, gain, offset);
    texColor.g = ToneCurve(texColor.g, gain, offset);
    texColor.b = ToneCurve(texColor.b, gain + 0.3f, offset);
    
    return float4(texColor.rgb, 1);

}