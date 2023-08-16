#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Transform.h"
#include "ModelManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "Camera.h"

class ColliderObject
{
private:
	std::vector<VertexBufferData::VObjModel> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VObjModel>> mVertexBuffer;
	std::vector<uint16_t> mIndices;
	std::unique_ptr<IndexBuffer> mIndexBuffer;
	Transform mTransform;
	Model* mModel;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

private:
	void GenerateSphere();

public:
	ColliderObject();
	void Update();
	void Draw();

public: //�Z�b�^�[

	// ���f��
	void SetModel(Model* model);

	// �O���t�B�b�N�X�p�C�v���C��
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// �J����
	void SetCamera(Camera* camera = nullptr);

public: // �Q�b�^�[

	// �g�����X�t�H�[��
	Transform GetTransform();

	// ���f��
	Model* GetModel();
};

