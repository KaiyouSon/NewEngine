#include "ObjectBasic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	float3 lightDir = float3(1, -1, 1);
	lightDir = normalize(lightDir);

	float3 lightColor = float3(1, 1, 1);

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mat, pos); // ���W�ɍs�����Z

	// Lambert���˂��v�Z
	output.color.rgb = dot(-lightDir, normal) * diffuse * lightColor;
	output.color.a = alpha;
	//output.normal = normal;
	output.uv = uv;
	return output;

	//VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	//output.svpos = mul(mat, pos); // ���W�ɍs�����Z
	//output.normal = normal;
	//output.uv = uv;
	//return output;
}