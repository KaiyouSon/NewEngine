// ランバート反射
float4 Lambert(float3 lightV, float3 normal)
{
    float3 lightDir = normalize(float3(lightV));
    float diffuse = saturate(dot(-lightDir, normal));
    float brightness = diffuse + 0.3f;
    float4 resultColor = float4(brightness, brightness, brightness, 1);
    return resultColor;
}

// フォーン反射
float3 Phone(float3 _lightV, float3 _normal, float3 _eyeDir, float _shininess, float3 _ambient, float3 _diffuse)
{
    // 頂点から視点へのベクトル
    float3 lightDir = normalize(_lightV);
    
    // ライトに向かうベクトルと法線の内積
    float3 dotLightNormal = dot(_lightV, _normal);
    
    // 反射光ベクトル
    float3 reflect = normalize(_lightV + 2 * dotLightNormal * _normal);
    
    // 環境反射光
    float3 ambient = _ambient;
    // 拡散反射光
    float3 diffuse = dotLightNormal * _diffuse;
    
    // 鏡面反射光
    float3 specular = pow(saturate(dot(reflect, _eyeDir)), _shininess);
    
    float3 resultColor = ambient + diffuse + specular;
    
    return resultColor;

}