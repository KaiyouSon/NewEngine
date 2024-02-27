#include "ObjModel.h"
namespace fs = std::filesystem;

ObjModel::ObjModel()
{
	format = ModelFormat::Obj;
}

ObjModel::ObjModel(const std::string tag, const std::string& path)
{
	format = ModelFormat::Obj;

	this->tag = tag;
	mPath = path;
}

void ObjModel::Create(const bool isSmoothing)
{
	//model->name = fileName;
	fs::path path = mPath;

	std::string filename = path.filename().string();

	std::string objfile = filename + ".obj";
	//uint32_t checkPos;
	// ファイルパスの最後に'/'があればファイル名だけにして".obj"を追加
	//checkPos = static_cast<uint32_t>(fileName.rfind('/'));
	//if (checkPos < fileName.size())
	//{
	//	objfile = fileName.substr(checkPos + 1, fileName.size() - checkPos - 1) + ".obj";
	//}

	//std::string path = GetInstance()->mDirectoryPath + fileName + "/";

	// ファイルを開いて読み込み
	std::ifstream file;
	// .objファイルを開く
	file.open(mPath + objfile);
	// ファイルが開けなかった場合
	if (file.fail())
	{
		assert(0 && "モデルデータの読み込みに失敗しました");
	}

	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		// 1行をスペースで分割
		std::istringstream lineStream(line);

		// 最初の単語を取得して、キーとする
		std::string key;
		std::getline(lineStream, key, ' ');

		if (key == "mtllib")
		{
			// マテリアル情報のファイル名を取得し、カラー情報を読み込む
			std::string mtlFileName;
			lineStream >> mtlFileName;

			//LoadMtlFile(path + mtlFileName, model.get());
			continue;
		}

		// ポジション情報を読み込む
		if (key == "v")
		{
			LoadPositions(lineStream, positions);
		}

		// テクスチャ座標情報を読み込む
		if (key == "vt")
		{
			LoadTexcoords(lineStream, texcoords);
		}

		// 法線情報を読み込む
		if (key == "vn")
		{
			LoadNormals(lineStream, normals);
		}

		// 面情報を読み込む
		if (key == "f")
		{
			// 面情報をスペースで分割
			std::string indexString;
			int count = 0;

			while (getline(lineStream, indexString, ' '))
			{
				// インデックスを解析
				std::istringstream indexStream(indexString);
				uint32_t indexPos = 0, indexNormal = 0, indexTexcoord = 0;

				indexStream >> indexPos;
				indexStream.seekg(1, std::ios_base::cur); // スラッシュをスキップ
				indexStream >> indexTexcoord;
				indexStream.seekg(1, std::ios_base::cur); // スラッシュをスキップ

				indexStream >> indexNormal;
				// ポジション情報、法線情報、テクスチャ座標情報を使って
				// モデルデータを構築
				VertexBufferData::VFbxModel vertex{};
				vertex.pos = positions[indexPos - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				mesh.AddVertex(vertex);

				// インデックスを追加
				if (count % 3 == 0)
				{
					mesh.AddIndex((uint16_t)mesh.indices.size());
				}
				if (count % 3 == 1)
				{
					mesh.AddIndex((uint16_t)mesh.indices.size() + 1);
				}
				if (count % 3 == 2)
				{
					mesh.AddIndex((uint16_t)mesh.indices.size() - 1);
				}

				if (isSmoothing == true)
				{
					mesh.AddSmoothData((uint16_t)indexPos, (uint16_t)mesh.indices.size() - 1);
				}

				count++;
			}
		}
	}

	// ファイルを閉じる
	file.close();

	if (isSmoothing == true)
	{
		mesh.CalculateSmoothedVertexNormals();
	}

	mesh.CreateBuffer();
}

void ObjModel::LoadPositions(std::istringstream& lineStream, std::vector<Vec3>& positions)
{
	// X,Y,Z座標を読み込む
	Vec3 pos{};
	lineStream >> pos.x;
	lineStream >> pos.y;
	lineStream >> pos.z;

	// Z軸を反転
	pos.z *= -1;

	// 座標データを配列に追加
	positions.emplace_back(pos);
}

void ObjModel::LoadTexcoords(std::istringstream& lineStream, std::vector<Vec2>& texcoords)
{
	// UV座標を読み込む
	Vec2 texcoord{};
	lineStream >> texcoord.x;
	lineStream >> texcoord.y;

	// Y座標を反転
	texcoord.y = 1.0f - texcoord.y;

	// テクスチャ座標データを配列に追加
	texcoords.emplace_back(texcoord);
}

void ObjModel::LoadNormals(std::istringstream& lineStream, std::vector<Vec3>& normals)
{
	// XYZ座標を読み込む
	Vec3 normal{};
	lineStream >> normal.x;
	lineStream >> normal.y;
	lineStream >> normal.z;

	// Z軸を反転
	normal.z *= -1;

	// 法線データを配列に追加
	normals.emplace_back(normal);
}
