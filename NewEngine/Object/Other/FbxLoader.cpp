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

	// スキニング情報の読み取り
	ParseSkin(fbxModel, fbxMesh);
}
void FbxLoader::ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	std::vector<VertexPosNormalUvBone>& vertices = fbxModel->mesh.vertices;

	// 頂点座標データの数
	const size_t controlPointsCount = fbxMesh->GetControlPointsCount();

	// 必要の数分メモリを確保
	VertexPosNormalUvBone vertex{};
	fbxModel->mesh.vertices.resize(controlPointsCount, vertex);

	// FBXメッシュの頂点座標配列を取得
	FbxVector4* pCoord = fbxMesh->GetControlPoints();

	// FBXメッシュの全頂点座標をモデル内の配列にコピーする
	for (size_t i = 0; i < controlPointsCount; i++)
	{
		VertexPosNormalUvBone& vertex = vertices[i];

		// 座標のコピー
		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}
}
void FbxLoader::ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	std::vector<VertexPosNormalUvBone>& vertices = fbxModel->mesh.vertices;
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
	for (int i = 0; i < polygonCount; i++)
	{
		// 面を構成する頂点の数を取得（3なら三角形ポリゴン）
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		// 1頂点ずつ処理
		for (int j = 0; j < polygonSize; j++)
		{
			// FBX頂点配列のインデックス
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			VertexPosNormalUvBone& vertex = vertices[index];

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
					vertex.uv.y = 1.f - (float)uvs[1];
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
				fbxModel->material.ambient.r = (float)ambient.Get()[0];
				fbxModel->material.ambient.g = (float)ambient.Get()[1];
				fbxModel->material.ambient.b = (float)ambient.Get()[2];

				// 拡散反射光
				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				fbxModel->material.diffuse.r = (float)diffuse.Get()[0];
				fbxModel->material.diffuse.g = (float)diffuse.Get()[1];
				fbxModel->material.diffuse.b = (float)diffuse.Get()[2];
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
void FbxLoader::ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	// スキニング情報
	FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

	// スキニング情報がなければ終了
	if (fbxSkin == nullptr)
	{
		return;
	}

	// ボーン配列の参照
	std::vector<FbxModelBone>& bones = fbxModel->bones;

	// ボーンの数
	int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);

	// 全てのボーンについて
	for (size_t i = 0; i < clusterCount; i++)
	{
		// FBXボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster((int)i);

		// ボーン自体のノードの名前を取得
		const char* boneName = fbxCluster->GetLink()->GetName();

		// 新しくボーンを追加し、追加したボーンの参照を得る
		bones.emplace_back(FbxModelBone(boneName));
		FbxModelBone& bone = bones.back();
		// 自作ボーンとFBXのボーンを紐づける
		bone.fbxCluster = fbxCluster;

		// FBXから初期姿勢行列を取得する
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		// Mat4型に変換する
		Mat4 initPoseMat;
		ConvertMat4FromFbx(&initPoseMat, fbxMat);

		// 初期姿勢行列の逆行列を得る
		bone.invInitPoseMat = initPoseMat.Inverse();
	}

	// ボーン番号とスキンウェイトのペア
	struct WeightSet
	{
		UINT index;
		float weight;
	};

	// 二次元配列（ジャグ配列）
	// list:頂点が影響を受けるボーンの全リスト
	// vector:それを全頂点分
	std::vector<std::list<WeightSet>> weightLists(fbxModel->mesh.vertices.size());

	// 全てのボーンについて
	for (size_t i = 0; i < clusterCount; i++)
	{
		// FBXボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster((int)i);

		// このボーンに影響を受ける頂点の数
		int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
		// このボーンに影響を受ける頂点の配列
		int* controlPointIndices = fbxCluster->GetControlPointIndices();
		double* controlPointWeights = fbxCluster->GetControlPointWeights();

		// 影響を受ける全頂点について
		for (size_t j = 0; j < controlPointIndicesCount; j++)
		{
			// 頂点番号
			int vertexIndex = controlPointIndices[j];
			// スキンウェイト
			float weight = (float)controlPointWeights[j];
			// その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
			weightLists[vertexIndex].emplace_back(WeightSet{ (UINT)i,weight });
		}
	}

	// 頂点配列書き変え用の参照
	auto& vertices = fbxModel->mesh.vertices;
	// 各頂点について処理
	for (size_t i = 0; i < vertices.size(); i++)
	{
		// 頂点のウェイトから最も大きい4つを選択
		auto& weightList = weightLists[i];
		// 大小比較用のラムダ式を指定して降順にソート
		weightList.sort(
			[](auto const& lhs, auto const& rhs)
			{
				return lhs.weight > rhs.weight;
			});

		int weightArrayIndex = 0;
		// 降順ソート済みのウェイトリストから
		for (auto& weightSet : weightList)
		{
			// 頂点データに書き込み
			vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
			vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;
			// 4つに達したら終了
			if (++weightArrayIndex >= maxBoneIndices)
			{
				float weight = 0.f;
				// 2番目以降のウェイトを合計
				for (size_t j = 1; j < maxBoneIndices; j++)
				{
					weight += vertices[i].boneWeight[j];
				}
				// 合計で1,f(100%)になるように調整
				vertices[i].boneWeight[0] = 1.f - weight;
				break;
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
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
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