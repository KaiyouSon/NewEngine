// ƒ‰ƒ“ƒo[ƒg”½Ë
float4 Lambert(float3 lightV, float3 normal)
{
    float3 lightDir = normalize(float3(lightV));
    float diffuse = saturate(dot(-lightDir, normal));
    float brightness = diffuse + 0.3f;
    return float4(brightness, brightness, brightness, 1);
}