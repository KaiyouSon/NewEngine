#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObject.h"

class Object3D : public GameObject
{
private:
	VertexBuffer<VertexPosNormalUv>* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* constantBuffer;

public:
	Object3D();
	~Object3D();
	void Initialize();
	void Update();
	void Draw();

	Mat4 GetFinalMat();
};

