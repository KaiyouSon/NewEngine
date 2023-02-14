#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <unordered_map>

class Mesh
{
private:
	// ���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;

public:
	VertexBuffer<VertexPosNormalUv> vertexBuffer;
	IndexBuffer indexBuffer;
	std::vector<VertexPosNormalUv> vertices;
	std::vector<unsigned short> indices;

public:

	// �X���[�W���O�f�[�^�̒ǉ�
	void AddSmoothData(const unsigned short& indexPos, const unsigned short& indexVertex);

	// Vertex�̒ǉ�
	void AddVertex(const VertexPosNormalUv& vertex);

	// Index�̒ǉ�
	void AddIndex(const unsigned short& index);

	// �o�b�t�@�[�̐���
	void CreateBuffer();

	// ���������ꂽ���_�@���̌v�Z
	void CalculateSmoothedVertexNormals();

public:
	// �Q�b�^�[
	inline size_t GetVertexSize() { return vertices.size(); }
	inline size_t GetIndexSize() { return indices.size(); }

	inline VertexBuffer<VertexPosNormalUv> GetVertexBuffer() { return vertexBuffer; }
	inline IndexBuffer GetIndexBuffer() { return indexBuffer; }
};

