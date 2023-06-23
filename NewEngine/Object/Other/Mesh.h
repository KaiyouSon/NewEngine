#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <unordered_map>

class Mesh
{
private:
	// ���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<uint16_t, std::vector<uint16_t>> smoothData;

public:
	std::vector<VertexBufferData::VFbxModel> vertices;
	VertexBuffer<VertexBufferData::VFbxModel> vertexBuffer;
	std::vector<uint16_t> indices;
	IndexBuffer indexBuffer;

public:
	// �X���[�W���O�f�[�^�̒ǉ�
	void AddSmoothData(const uint16_t indexPos, const uint16_t indexVertex);

	// Vertex�̒ǉ�
	void AddVertex(const VertexBufferData::VFbxModel vertex);

	// Index�̒ǉ�
	void AddIndex(const uint16_t index);

	// �o�b�t�@�[�̐���
	void CreateBuffer();

	// ���������ꂽ���_�@���̌v�Z
	void CalculateSmoothedVertexNormals();
};

