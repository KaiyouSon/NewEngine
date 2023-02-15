#include "FbxLoader.h"
#include "MathUtil.h"
#include "VertexBufferData.h"
#include "TextureManager.h"
#include <cassert>
#include <DirectXTex.h>

const std::string FbxLoader::baseDirectory = "Application/Resources/Model/";
const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

void FbxLoader::Init()
{
	assert(fbxManager == nullptr);

	// 生成
	fbxManager = FbxManager::Create();

	// 入出力設定
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	// インポーターの生成
	fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Destroy()
{
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

FbxModel* FbxLoader::Load(const std::string& modelName)
{
	// モデルと同じ名前のフォルダーから読み込む
	std::string path = "Application/Resources/Model/" + modelName + "/";
	std::string fbxfile = modelName + ".fbx";
	std::string fullPath = path + fbxfile;

	if (!fbxImporter->Initialize(fullPath.c_str(), -1, fbxManager->GetIOSettings()))
	{
		assert(0 && "モデルの読み込みが失敗しました");
	}

	// シーン生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

	// ファイルからロードしたFBXの情報をシーンインポート
	fbxImporter->Import(fbxScene);

	// モデル生成
	FbxModel* fbxModel = new FbxModel;
	fbxModel->name = modelName;

	// fbxノード数を取得して必要サイズ分メモリ確保する
	const size_t nodeCount = fbxScene->GetNodeCount();
	fbxModel->nodes.reserve(nodeCount);

	// ルートノードから順に解析してモデルに流し込む
	ParseNodeRecursive(fbxModel, fbxScene->GetRootNode());
	// fbxシーンの解放
	fbxScene->Destroy();

	fbxModel->mesh.vertexBuffer.Create(fbxModel->mesh.vertices);
	fbxModel->mesh.indexBuffer.Create(fbxModel->mesh.indices);

	return fbxModel;
}

void FbxLoader::ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode)
{
	// ノードのメッシュを取得
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	// 頂点座標読み取り
	ParseMeshVertices(fbxModel, fbxMesh);

	// 面を構成するデータの読み取り
	ParseMeshFaces(fbxModel, fbxMesh);

	// マテリアルの読み取り
	ParseMaterial(fbxModel, fbxNode);
}

void FbxLoader::ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	std::vector<VertexPosNormalUv>& vertices = fbxModel->mesh.vertices;

	// 頂点座標データの数
	const size_t controlPointsCount = fbxMesh->GetControlPointsCount();

	// 必要の数分メモリを確保
	VertexPosNormalUv vert{};
	fbxModel->mesh.vertices.resize(controlPointsCount, vert);

	// FBXメッシュの頂点座標配列を取得
	FbxVector4* pCoord = fbxMesh->GetControlPoints();

	// FBXメッシュの全頂点座標をモデル内の配列にコピーする
	for (size_t i = 0; i < controlPointsCount; i++)
	{
		VertexPosNormalUv& vertex = vertices[i];

		// 座標のコピー
		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}
}

void FbxLoader::ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	std::vector<VertexPosNormalUv>& vertices = fbxModel->mesh.vertices;
	std::vector<unsigned short>& indices = fbxModel->mesh.indices;

	// 1ファイルに複数メッシュのモデルは否対応
	if (indices.size() > 0)
		//if (indices.size() == 0)
	{
		assert(0 && "1ファイルに複数メッシュのモデルは否対応");
	}

	// 面の数
	const int polygonCount = fbxMesh->GetPolygonCount();
	// UVデータの数
	const int textureUVCount = fbxMesh->GetTextureUVCount();
	// UV名リスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	// 面ごとの情報読み取り
	for (size_t i = 0; i < polygonCount; i++)
	{
		// 面を構成する頂点の数を取得（3なら三角形ポリゴン）
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		// 1頂点ずつ処理
		for (size_t j = 0; j < polygonSize; j++)
		{
			// FBX頂点配列のインデックス
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			VertexPosNormalUv& vertex = vertices[index];

			// 頂点法線の読み込み
			FbxVector4 normal;
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}

			// テクスチャーUV読み込み
			if (textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUv;
				// 0番決め打ちで読み込む
				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUv))
				{
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}
			}

			// インデックス配列に頂点インデックス追加
			if (j < 3)	// 3頂点目までなら
			{
				indices.push_back(index);
			}
			else		// 4頂点目
			{
				// 3点追加し、四角形の0,1,2,3の内、2,3,0で三角形を構築する
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];

				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}
}

void FbxLoader::ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0)
	{
		// 先頭マテリアルを取得
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

		if (material)
		{
			// FbxSurfaceLambertクラスかどうかを調べる
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

				// 環境光
				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				fbxModel->material.ambient.x = (float)ambient.Get()[0];
				fbxModel->material.ambient.y = (float)ambient.Get()[1];
				fbxModel->material.ambient.z = (float)ambient.Get()[2];

				// 拡散反射光
				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				fbxModel->material.diffuse.x = (float)diffuse.Get()[0];
				fbxModel->material.diffuse.y = (float)diffuse.Get()[1];
				fbxModel->material.diffuse.z = (float)diffuse.Get()[2];
			}
		}

		// ディフューズテクスチャを取り出す
		const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (diffuseProperty.IsValid())
		{
			const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
			if (texture)
			{
				const char* filePath = texture->GetFileName();
				// ファイルパスからファイル名を抽出
				std::string pathStr(filePath);
				std::string name = ExractFileName(pathStr);
				std::string fullPath = baseDirectory + fbxModel->name + "/" + name;
				// テクスチャ読み込み
				fbxModel->material.texture =
					TextureManager::LoadMaterialTexture(fullPath);
			}
		}
	}
}

void FbxLoader::ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, FbxModelNode* parent)
{
	// ノード名を取得
	std::string nodeName = fbxNode->GetName();

	// モデルにノードを追加
	fbxModel->nodes.emplace_back();
	FbxModelNode& fbxModelNode = fbxModel->nodes.back();

	// ノード名
	fbxModelNode.name = fbxNode->GetName();

	// ローカル情報
	FbxDouble3 pos = fbxNode->LclTranslation.Get();
	FbxDouble3 scale = fbxNode->LclScaling.Get();
	FbxDouble3 rot = fbxNode->LclRotation.Get();

	fbxModelNode.pos = { (float)pos[0],(float)pos[1],(float)pos[2] };
	fbxModelNode.scale = { (float)scale[0],(float)scale[1],(float)scale[2] };
	fbxModelNode.rot = { (float)rot[0],(float)rot[1],(float)rot[2] };
	fbxModelNode.rot = Radian(fbxModelNode.rot);

	// 平行移動、スケーリング、回転行列計算
	Mat4 transMat = Mat4::Identity();
	Mat4 scaleMat = Mat4::Identity();
	Mat4 rotMat = Mat4::Identity();

	transMat = ConvertTranslationMat(fbxModelNode.pos);		// 平行移動
	scaleMat = ConvertScalingMat(fbxModelNode.scale);		// スケーリング
	rotMat *= ConvertRotationZAxisMat(fbxModelNode.rot.z);	// z軸回転
	rotMat *= ConvertRotationXAxisMat(fbxModelNode.rot.x);	// x軸回転
	rotMat *= ConvertRotationYAxisMat(fbxModelNode.rot.y);	// y軸回転

	fbxModelNode.localTransformMat = Mat4::Identity();
	fbxModelNode.localTransformMat *= scaleMat;
	fbxModelNode.localTransformMat *= rotMat;
	fbxModelNode.localTransformMat *= transMat;

	// グローバル変形行列の計算
	fbxModelNode.globalTransformMat = fbxModelNode.localTransformMat;
	if (parent)
	{
		fbxModelNode.parent = parent;
		// 親の変形を乗算
		fbxModelNode.globalTransformMat *= parent->globalTransformMat;
	}

	// FBXノードのメッシュ情報を解析
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

	if (fbxNodeAttribute)
	{
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			fbxModel->meshNode = &fbxModelNode;
			ParseMesh(fbxModel, fbxNode);
		}
	}

	// 再帰
	for (size_t i = 0; i < fbxNode->GetChildCount(); i++)
	{
		ParseNodeRecursive(fbxModel, fbxNode->GetChild(i), &fbxModelNode);
	}
}

std::string FbxLoader::ExractFileName(const std::string& path)
{
	size_t pos1;
	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}
