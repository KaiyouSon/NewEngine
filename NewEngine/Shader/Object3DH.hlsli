// 3D�ϊ��s��
cbuffer ConstantBufferDataTransform : register(b0)
{
	//matrix mat;// 3D�ϊ��s��
	matrix viewMat;
	matrix worldMat;
	float3 cameraPos;
}

// �}�e���A��
cbuffer ConstantBufferDataMaterial : register(b1)
{
	float3 ambient : packoffset(c0);	// �A���r�G���g�W��
	float3 diffuse : packoffset(c1);	// �f�B�t���[�Y�W��
	float3 specular : packoffset(c2);	// �X�y�L�����[�W��
	float alpha : packoffset(c2.w);		// �A���t�@
}

// �F
cbuffer ConstantBufferDataColor : register(b2)
{
	float4 color;	// �F
}

// --- ���s���� ---------------------------- //
static const int DirectionalLightNum = 1;
struct DirectionalLight
{
	float3 lightVec;	// ����
	float3 color;		// �F
	uint isActive;
};

// --- �_���� ------------------------------ //
static const int PointLightNum = 3;
struct PointLight
{
	float3 pos;		// ���W
	float3 color;	// �F
	float3 atten;	// ���C�g���������W��
	uint isActive;
};

// --- �X�|�b�g���C�g ---------------------- //
static const int spotLightNum = 3;
struct SpotLight
{
	float3 vec;		// �t�x�N�g��
	float3 pos;		// ���W
	float3 color;	// �F
	float3 atten;	// ���C�g���������W��
	float2 factorAngleCos;	// ���C�g�̌����p�x�̃R�T�C��
	uint isActive;
};

cbuffer ConstantBufferDataLightManager : register(b3)
{
	DirectionalLight directionalLights[DirectionalLightNum];
	PointLight pointLights[PointLightNum];
	SpotLight spotLights[spotLightNum];
};

// --- �t�H�O ---------------------- //
cbuffer ConstantBufferDataFog : register(b4)
{
	uint isActiveFog;
	float fogNearDis;
	float fogFarDis;
	float4 fogColor;
};

// --- �ۉe ----------------------------------------------------- //
static const int circleShadowNum = 1;
struct CircleShadow
{
	float3 vec;		// �t�x�N�g��
	float3 pos;		// ���W
	float disCasterLight;	// �L���X�^�[�ƃ��C�g�̋���
	float3 atten;	// ���������W��
	float2 factorAngleCos;	// �����p�x�̃R�T�C��
	uint isActive;
};

cbuffer ConstantBufferDataCircleShadow : register(b6)
{
	CircleShadow circleShadows[circleShadowNum];
}

// ���_�V�F�[�_�[�̏o�͍\����
struct VSOutput
{
	float4 svpos : SV_POSITION;	// �V�X�e���p���_���W
	float4 worldPos : POSITION;	// ���[���h���W
	float3 normal : NORMAL;		// �@���x�N�g��
	float2 uv : TEXCOORD;		// uv�l
};