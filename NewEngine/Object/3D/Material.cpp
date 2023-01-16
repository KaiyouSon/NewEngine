#include "Material.h"
#include <fstream>
#include <sstream>
#include <cassert>
using namespace std;

Material::Material() :
	ambient(0.3f, 0.3f, 0.3f), diffuse(0.0f, 0.0f, 0.0f),
	specular(0.0f, 0.0f, 0.0f), alpha(1.0f), texture(Texture(Color::white))
{
}
Material::Material(std::string filePath) :
	ambient(0.3f, 0.3f, 0.3f), diffuse(0.0f, 0.0f, 0.0f),
	specular(0.0f, 0.0f, 0.0f), alpha(1.0f), texture(Texture(Color::white))
{
	// ファイルストリーム
	ifstream file;
	// .objファイルを開く
	file.open(filePath);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0 && "マテリアルの読み込みが失敗しました");
	}

	string directoryPath = filePath;
	while (true)
	{
		directoryPath.pop_back();
		if (directoryPath.back() == '/')
		{
			break;
		}
	}

	// 1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(lineStream, key, ' ');

		// 先頭のタブ文字を無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			// マテリアル名読み込み
			lineStream >> name;
		}

		// 先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			lineStream >> ambient.x;
			lineStream >> ambient.y;
			lineStream >> ambient.z;
		}

		// 先頭文字列がKaならディフーズ色
		if (key == "Kd")
		{
			lineStream >> diffuse.x;
			lineStream >> diffuse.y;
			lineStream >> diffuse.z;
		}

		// 先頭文字列がKaならスペキュラー色
		if (key == "Ks")
		{
			lineStream >> specular.x;
			lineStream >> specular.y;
			lineStream >> specular.z;
		}

		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			// テクスチャのファイル名読み込み
			string textureName;
			lineStream >> textureName;

			// テクスチャ読み込み
			texture = Texture(directoryPath + textureName, true);
			texture.isMaterial = true;
		}
	}

	// ファイルを閉じる
	file.close();
}
