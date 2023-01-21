#include "Object3DH.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	{
		// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p		
		float4 wnormal = normalize(mul(worldMat, float4(normal, 0)));
		float4 wpos = mul(worldMat, pos);

		VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
		output.svpos = mul(mul(viewMat, worldMat), pos);
		output.worldPos = wpos;
		output.normal = wnormal.xyz;
		output.uv = uv;

		return output;
	}


	//{
	//	//// �E�A���A���̕��������������C�g
	//	//float3 lightDir = float3(1, -1, 1);
	//	//lightDir = normalize(lightDir);

	//	//// ���C�g�̐F
	//	//float3 lightColor = float3(1, 1, 1);

	//	float4 wnormal = normalize(mul(worldMat, float4(normal, 0)));
	//	float4 wpos = mul(worldMat, pos);

	//	// �s�N�Z���V�F�[�_�[�ɓn���l
	//	VSOutput output;
	//	//output.svpos = mul(mat, pos); // ���W�ɍs�����Z
	//	output.svpos = mul(mul(viewMat, worldMat), pos);

	//	// Phone����
	//	{
	//		// �����ˌ�
	//		float3 tAmbient = ambient;

	//		// �g�U���ˌ�
	//		//float3 tDiffuse = dot(-lightDir, normal) * diffuse;
	//		float3 tDiffuse = dot(lightVec, wnormal.xyz) * diffuse;

	//		// ���_���W
	//		const float3 eye = float3(0, 0, -20);
	//		// ����x
	//		const float shininess = 4.0f;
	//		// ���_���王�_�ւ̃x�N�g��
	//		//float3 eyeDir = normalize(eye - pos.xyz);
	//		float3 eyeDir = normalize(cameraPos - wpos.xyz);
	//		// ���ˌ��x�N�g��
	//		//float reflect = normalize(lightDir + 2 * dot(-lightDir, normal) * normal);
	//		float3 reflect = normalize(-lightVec + 2 * dot(lightVec, wnormal.xyz) * wnormal.xyz);
	//		// ���ʔ��ˌ�
	//		float tSpecular = pow(saturate(dot(reflect, eyeDir)), shininess) * specular;

	//		output.color.rgb = (tAmbient + tDiffuse + tSpecular) * lightColor;
	//		output.color.a = alpha;
	//	}

	//	// Lambert����
	//	{
	//		// Lambert���˂̌v�Z
	//		//output.color.rgb = dot(-lightDir, normal) * diffuse, lightColor;
	//		//output.color.a = alpha;
	//	}
	//	output.uv = uv;

	//	return output;
	//}

	//VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	//output.svpos = mul(mat, pos); // ���W�ɍs�����Z
	//output.normal = normal;
	//output.uv = uv;
	//return output;
}