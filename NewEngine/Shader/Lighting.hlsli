// �����o�[�g����
float4 Lambert(float3 lightV, float3 normal)
{
    float3 lightDir = normalize(float3(lightV));
    float diffuse = saturate(dot(-lightDir, normal));
    float brightness = diffuse + 0.3f;
    float4 resultColor = float4(brightness, brightness, brightness, 1);
    return resultColor;
}

// �t�H�[������
float3 Phone(float3 _lightV, float3 _normal, float3 _eyeDir, float _shininess, float3 _ambient, float3 _diffuse)
{
    // ���_���王�_�ւ̃x�N�g��
    float3 lightDir = normalize(_lightV);
    
    // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
    float3 dotLightNormal = dot(_lightV, _normal);
    
    // ���ˌ��x�N�g��
    float3 reflect = normalize(_lightV + 2 * dotLightNormal * _normal);
    
    // �����ˌ�
    float3 ambient = _ambient;
    // �g�U���ˌ�
    float3 diffuse = dotLightNormal * _diffuse;
    
    // ���ʔ��ˌ�
    float3 specular = pow(saturate(dot(reflect, _eyeDir)), _shininess);
    
    float3 resultColor = ambient + diffuse + specular;
    
    return resultColor;

}