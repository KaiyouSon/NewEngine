// �\����

// �}�e���A��
struct Material
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

// ���s����
struct DirectionalLight
{
    float3 vec; // ����
    float3 color; // �F
    uint isActive;
};

// �_����
struct PointLight
{
    float3 pos; // ���W
    float3 color; // �F
    float3 atten; // ���C�g���������W��
    uint isActive;
};

// �X�|�b�g���C�g
struct SpotLight
{
    float3 vec; // �t�x�N�g��
    float3 pos; // ���W
    float3 color; // �F
    float3 atten; // ���C�g���������W��
    float2 factorAngleCos; // ���C�g�̌����p�x�̃R�T�C��
    uint isActive;
};

// �����v�Z�p�f�[�^
struct LightCalculateData
{
    float3 vertexPos;
    float3 vertexNormal;
    float3 lightVec;
    float3 lightNormal;
};

// �ۉe
struct CircleShadow
{
    float3 vec; // �t�x�N�g��
    float3 pos; // ���W
    float disCasterLight; // �L���X�^�[�ƃ��C�g�̋���
    float3 atten; // ���������W��
    float2 factorAngleCos; // �����p�x�̃R�T�C��
    uint isActive;
};

// �t�H�O
struct Fog
{
    uint isActive;
    float nearDis;
    float farDis;
    float4 color;
};

// �����o�[�g����
float3 Lambert(float3 lightV, float3 normal)
{
    float3 lightDir = normalize(float3(lightV));
    float diffuse = saturate(dot(-lightDir, normal));
    float brightness = diffuse + 0.3f;
    float3 resultColor = brightness;
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

// ���s����
float3 CalculateDirectionalLight(DirectionalLight _directionalLight, Material _material,
    float3 _normal, float3 _lightNormal, float3 _eyeDir, float _shininess)
{
    float3 resultColor = 0;
    
    if (_directionalLight.isActive == (uint) true)
    {
		// ���ˌ��x�N�g��
        float3 reflect = normalize(-_directionalLight.vec + 2 * _lightNormal * _normal);

		// �g�U���ˌ�
        float3 diffuse = _lightNormal * _material.diffuse;
		// ���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, _eyeDir)), _shininess) * _material.specular;

		// �S�ĉ��Z����
        resultColor.rgb += (diffuse + specular) * _directionalLight.color;
    }
    
    return resultColor;
}

// �_����
float3 CalculatePointLight(PointLight _pointLight, Material _material,
    LightCalculateData _lightCalculateData, float3 _eyeDir, float _shininess)
{
    float3 resultColor = 0;
    
    if (_pointLight.isActive == (uint) true)
    {
		// �x�N�g���̒���
        float d = length(_lightCalculateData.lightVec);

		// ���������W��
        float atten = 1.0f /
				(_pointLight.atten.x +
				_pointLight.atten.y * d +
				_pointLight.atten.z * d * d);

		// ���ˌ��x�N�g��
        float3 reflect = normalize(
        -_lightCalculateData.lightVec + 2 *
        _lightCalculateData.lightNormal *
        _lightCalculateData.vertexNormal);
        
		// �g�U���ˌ�
        float3 diffuse = _lightCalculateData.lightNormal * _material.diffuse;
		// ���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, _eyeDir)), _shininess) * _material.specular;

		// �S�ĉ��Z����
        resultColor += atten * (diffuse + specular) * _pointLight.color;
    }
    
    return resultColor;
}

// �X�|�b�g���C�g
float3 CalculateSpotLight(SpotLight _spotLight, Material _material,
    LightCalculateData _lightCalculateData, float3 _eyeDir, float _shininess)
{
    float3 resultColor = 0;
    
    if (_spotLight.isActive == (uint) true)
    {
		// �x�N�g���̒���
        float d = length(_lightCalculateData.lightVec);

		// ���������W��
        float atten = saturate(1.0f /
				(_spotLight.atten.x +
					_spotLight.atten.y * d +
					_spotLight.atten.z * d * d));

		// �p�x����
        float cos = dot(_lightCalculateData.lightVec, _spotLight.vec);
		// �����J�n�p�x����A�����I���p�x�ɂ����Č���
		// �����J�n�p�x�̓����͂P�{�A�����I���p�x�̊O���͂O�{�̋P�x
        float angleAtten = smoothstep(_spotLight.factorAngleCos.y, _spotLight.factorAngleCos.x, cos);
		// �p�x��������Z
        atten *= angleAtten;

		// ���ˌ��x�N�g��
        float3 reflect = normalize(
            -_lightCalculateData.lightVec + 2 *
            _lightCalculateData.lightNormal *
            _lightCalculateData.vertexNormal);

		// �g�U���ˌ�
        float3 diffuse = _lightCalculateData.lightNormal * _material.diffuse;
		// ���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, _eyeDir)), _shininess) * _material.specular;

	    // �S�ĉ��Z����
        resultColor += atten * (diffuse + specular) * _spotLight.color;
    }

    return resultColor;
}

// �ۉe
float3 CalculateCircleShadow(CircleShadow _circleShadow, float3 _pos)
{
    float3 resultColor = 0;
    
    if (_circleShadow.isActive == (uint) true)
    {
			// �I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
        float3 casterV = _circleShadow.pos - _pos;

			// ���e�����ł̋���
        float d = dot(casterV, _circleShadow.vec);

			// ���������W��
        float atten = saturate(1.0f /
				(_circleShadow.atten.x +
					_circleShadow.atten.y * d +
					_circleShadow.atten.z * d * d));
			// �������}�C�i�X�Ȃ�O�ɂ���
        atten *= step(0, d);

			// ���z���C�g�̍��W
        float3 lightPos = _circleShadow.pos + _circleShadow.vec * _circleShadow.disCasterLight;

			// �I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g��
        float3 lightV = normalize(lightPos - _pos);

			// �p�x�v�Z
        float cos = dot(lightV, _circleShadow.vec);
			// �����J�n�p�x����A�����I���p�x�ɂ����Č���
			// �����J�n�p�x�̓����͂P�{�A�����I���p�x�̊O���͂O�{�̋P�x
        float angleAtten = smoothstep(_circleShadow.factorAngleCos.y, _circleShadow.factorAngleCos.x, cos);
			// �p�x��������Z
        atten *= angleAtten;

		// �S�Č��Z����
        resultColor -= atten;
    }
    
    return resultColor;
}

float4 CalculateFog(Fog fog, float dis, float4 currentColor)
{
    if (fog.isActive == (uint) true)
    {
		// �J�����̍��W�ƒ��_�̋���
        float rate = smoothstep(fog.nearDis, fog.farDis, dis);

		// �t�H�O�̐F
        return fog.color * rate + currentColor * (1 - rate);
    }
    
    return currentColor;
}