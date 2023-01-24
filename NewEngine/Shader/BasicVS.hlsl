#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//float4 wnormal = normalize(mul(worldMat, float4(normal, 0)));
	//float4 wpos = mul(worldMat, pos);

	//VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	//output.svpos = mul(mul(viewMat, worldMat), pos);
	//float3 eyeDir = normalize(cameraPos - wpos.xyz);

	//// �g�U���ˌ�
	//float3 diffuse = dot(lightVec, wnormal.xyz) * diffuse;

	//float3 reflect = normalize(-lightVec + 2 * dot(lightVec, wnormal.xyz) * wnormal.xyz);

	//return output;

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mat, pos); // ���W�ɍs�����Z
	output.normal = normal;
	output.uv = uv;
	return output;
}