#include "Model.h"
#include <fstream>
#include <sstream>
using namespace std;

Model::Model(string modelName)
{
	string path = "Application/Resources/Model/" + modelName + "/";
	string objfile = modelName + ".obj";

	// ファイルストリーム
	ifstream file;
	// .objファイルを開く
	file.open(path + objfile);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0 && "モデルの読み込みが失敗しました");
	}
	vector<Vec3> positions;
	vector<Vec3> normals;
	vector<Vec2> texcoords;

	// 1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(lineStream, key, ' ');

		if (key == "mtllib")
		{
			// マテリアルのファイル名を読み込む
			string fileName;
			lineStream >> fileName;

			material = Material(path + fileName);
			continue;
		}

		// 先頭文字列がvなら頂点座標
		if (key == "v")
		{
			// X,Y,Z座標読み込み
			Vec3 pos{};
			lineStream >> pos.x;
			lineStream >> pos.y;
			lineStream >> pos.z;
			// 座標データに追加
			positions.emplace_back(pos);
		}

		// 先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			// uv成分読み込み
			Vec2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;
			// v方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		// 先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			// xyz成分読み込み
			Vec3 normal{};
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		// 先頭文字列がfならポリゴン(三角形)
		if (key == "f")
		{
			// 半角スペース区切りで行の続きを読み込む
			string indexString;
			while (getline(lineStream, indexString, ' '))
			{
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				istringstream indexStream(indexString);
				unsigned short indexPos, indexNormal, indexTexcoord;
				indexStream >> indexPos;
				indexStream.seekg(1, ios_base::cur);	// スラッシュを飛ばす
				indexStream >> indexTexcoord;
				indexStream.seekg(1, ios_base::cur);	// スラッシュを飛ばす
				indexStream >> indexNormal;
				// 頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPos - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				// 頂点インデックスに追加
				indices.emplace_back((unsigned short)indices.size());
			}
		}
	}

	// ファイルを閉じる
	file.close();

	// 頂点バッファ
	vertexBuffer.Initialize(vertices);
	// インデックスバッファ
	indexBuffer.Initialize(indices);
}