#include "AssimpLoader.h"
#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
#include <assimp/postprocess.h>
void AssimpLoader::Init()
{
	//aiMesh mesh;


}

void AssimpLoader::LoadFbxModel()
{
	// インポーター
	//Assimp::Importer importer;

	//// フラグ
	//uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
	//const aiScene* scene = importer.ReadFile("cube/cube.fbx", flags);
}

//void AssimpLoader::ParseMesh(aiMesh* mesh)
//{
//	// 頂点座標
//	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
//	{
//		aiVector3D vertex = mesh->mVertices[i];
//	}
//}