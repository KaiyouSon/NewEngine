#include "Particle.hlsli"

// 四角形の頂点数
static const uint vnum = 4;

// センターからのオフセット
static const float4 offsetArray[vnum] =
{
	//float4(-0.5f,-0.5f,0,0),	// 左下
	//float4(-0.5f,+0.5f,0,0),	// 左上
	//float4(+0.5f,-0.5f,0,0),	// 右下
	//float4(+0.5f,+0.5f,0,0),	// 右上

	float4(-1.f,-1.f,0,0),	// 左下
	float4(-1.f,+1.f,0,0),	// 左上
	float4(+1.f,-1.f,0,0),	// 右下
	float4(+1.f,+1.f,0,0),	// 右上
};

// uv割り当て
static const float2 uvArray[vnum] =
{
	float2(0,1),	// 左下
	float2(0,0),	// 左上
	float2(1,1),	// 右下
	float2(1,0),	// 右上
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output)
{
	GSOutput element;
	// 頂点数分回す
	for (uint i = 0; i < vnum; i++)
	{
		float4 offset = offsetArray[i] * input[0].scale;

		// ワールド座標ベースでずらす
		element.svpos = input[0].pos + offset;

		// ビュー変換、射影変換
		element.svpos = mul(mat, element.svpos);
		element.uv = uvArray[i];
		element.color = input[0].color;
		output.Append(element);
	}
}