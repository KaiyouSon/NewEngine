#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "MathUtil.h"
#include <vector>
#include <memory>
#include <d3dx12.h>
#include <wrl.h>

class RenderTexture : public Texture
{
private:
	HRESULT result;

	RenderTarget renderTarget;
	DepthBuffer depthBuffer;
	Transform transform;

	static const float clearColor[4];

public:
	RenderTexture();

	void PreDrawScene();
	void PostDrawScene();
};