#include "ModelManager.h"
#include "TextureManager.h"
#include "AssimpLoader.h"
#include <fstream>
#include <sstream>

using namespace VertexBufferData;

// 繝｢繝・Ν縺ｮ繝槭ャ繝・
std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::sModelMap;
std::mutex ModelManager::sMtx = std::mutex{};
std::string ModelManager::sDirectoryPath = "Application/Resources/Model/";

// 繝｢繝・Ν縺ｮ蜿門ｾ・
Model* ModelManager::GetModel(const std::string modelTag)
{
	std::string log;
	if (sModelMap[modelTag].get() == nullptr)
	{
		log = "[Model Use] ModelTag : " + modelTag + ", does not exist";
	}
	else
	{
		log = "[Model Use] ModelTag : " + modelTag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return sModelMap[modelTag].get();
}

// obj繝輔ぃ繧､繝ｫ縺九ｉ繝｢繝・Ν繧偵Ο繝ｼ繝峨＠繝槭ャ繝励・譬ｼ邏阪☆繧・
Model* ModelManager::LoadObjModel(const std::string fileName, const std::string modelTag, const bool isSmoothing)
{
	// 謗剃ｻ門宛蠕｡
	std::lock_guard<std::mutex> lock(sMtx);

	// 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ逕滓・
	std::unique_ptr<Model> model = std::make_unique<ObjModel>();
	model->name = fileName;

	std::string objfile = fileName + ".obj";
	uint32_t checkPos;
	// 蛹ｺ蛻・ｊ譁・ｭ・'/' 縺悟・縺ｦ縺上ｋ荳逡ｪ譛蠕後・驛ｨ蛻・ｒ讀懃ｴ｢
	checkPos = static_cast<uint32_t>(fileName.rfind('/'));
	if (checkPos < fileName.size())
	{
		objfile = fileName.substr(checkPos + 1, fileName.size() - checkPos - 1) + ".obj";
	}

	std::string path = sDirectoryPath + fileName + "/";

	// 繝輔ぃ繧､繝ｫ繧ｹ繝医Μ繝ｼ繝
	std::ifstream file;
	// .obj繝輔ぃ繧､繝ｫ繧帝幕縺・
	file.open(path + objfile);
	// 繝輔ぃ繧､繝ｫ繧ｪ繝ｼ繝励Φ螟ｱ謨励ｒ繝√ぉ繝・け
	if (file.fail())
	{
		std::string log = "[ObjModel Load] FileName : " + fileName + ", Tag : " + modelTag + ", is,failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "繝｢繝・Ν縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ縺悟､ｱ謨励＠縺ｾ縺励◆");
	}

	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;

	// 1陦後★縺､隱ｭ縺ｿ霎ｼ繧
	std::string line;
	while (getline(file, line))
	{
		// 1陦悟・縺ｮ譁・ｭ怜・繧偵せ繝医Μ繝ｼ繝縺ｫ螟画鋤縺励※隗｣譫舌＠繧・☆縺上☆繧・
		std::istringstream lineStream(line);

		// 蜊願ｧ偵せ繝壹・繧ｹ蛹ｺ蛻・ｊ縺ｧ陦後・蜈磯ｭ譁・ｭ怜・繧貞叙蠕・
		std::string key;
		std::getline(lineStream, key, ' ');

		if (key == "mtllib")
		{
			// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ繝輔ぃ繧､繝ｫ蜷阪ｒ隱ｭ縺ｿ霎ｼ繧
			std::string mtlFileName;
			lineStream >> mtlFileName;

			LoadMaterialColor(path + mtlFileName, model.get());
			continue;
		}

		// 蜈磯ｭ譁・ｭ怜・縺計縺ｪ繧蛾らせ蠎ｧ讓・
		if (key == "v")
		{
			// X,Y,Z蠎ｧ讓呵ｪｭ縺ｿ霎ｼ縺ｿ
			Vec3 pos{};
			lineStream >> pos.x;
			lineStream >> pos.y;
			lineStream >> pos.z;
			pos.z *= -1;
			// 蠎ｧ讓吶ョ繝ｼ繧ｿ縺ｫ霑ｽ蜉
			positions.emplace_back(pos);
		}

		// 蜈磯ｭ譁・ｭ怜・縺計t縺ｪ繧峨ユ繧ｯ繧ｹ繝√Ε
		if (key == "vt")
		{
			// uv謌仙・隱ｭ縺ｿ霎ｼ縺ｿ
			Vec2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;
			// v譁ｹ蜷大渚霆｢
			texcoord.y = 1.0f - texcoord.y;
			// 繝・け繧ｹ繝√Ε蠎ｧ讓吶ョ繝ｼ繧ｿ縺ｫ霑ｽ蜉
			texcoords.emplace_back(texcoord);
		}

		// 蜈磯ｭ譁・ｭ怜・縺計n縺ｪ繧画ｳ慕ｷ壹・繧ｯ繝医Ν
		if (key == "vn")
		{
			// xyz謌仙・隱ｭ縺ｿ霎ｼ縺ｿ
			Vec3 normal{};
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;
			normal.z *= -1;
			// 豕慕ｷ壹・繧ｯ繝医Ν繝・・繧ｿ縺ｫ霑ｽ蜉
			normals.emplace_back(normal);
		}

		// 蜈磯ｭ譁・ｭ怜・縺掲縺ｪ繧峨・繝ｪ繧ｴ繝ｳ(荳芽ｧ貞ｽ｢)
		if (key == "f")
		{
			// 蜊願ｧ偵せ繝壹・繧ｹ蛹ｺ蛻・ｊ縺ｧ陦後・邯壹″繧定ｪｭ縺ｿ霎ｼ繧
			std::string indexString;
			int count = 0;

			while (getline(lineStream, indexString, ' '))
			{
				// 鬆らせ繧､繝ｳ繝・ャ繧ｯ繧ｹ1蛟句・縺ｮ譁・ｭ怜・繧偵せ繝医Μ繝ｼ繝縺ｫ螟画鋤縺励※隗｣譫舌＠繧・☆縺上☆繧・
				std::istringstream indexStream(indexString);
				uint32_t indexPos = 0, indexNormal = 0, indexTexcoord = 0;

				indexStream >> indexPos;
				indexStream.seekg(1, std::ios_base::cur);	// 繧ｹ繝ｩ繝・す繝･繧帝｣帙・縺・
				indexStream >> indexTexcoord;
				indexStream.seekg(1, std::ios_base::cur);	// 繧ｹ繝ｩ繝・す繝･繧帝｣帙・縺・

				indexStream >> indexNormal;
				// 鬆らせ繝・・繧ｿ縺ｮ霑ｽ蜉
				VFbxModel vertex{};
				vertex.pos = positions[indexPos - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];

				model->mesh.AddVertex(vertex);

				// 鬆らせ繧､繝ｳ繝・ャ繧ｯ繧ｹ縺ｫ霑ｽ蜉
				if (count % 3 == 0)
				{
					model->mesh.AddIndex((uint16_t)model->mesh.indices.size());
				}
				if (count % 3 == 1)
				{
					model->mesh.AddIndex((uint16_t)model->mesh.indices.size() + 1);
				}
				if (count % 3 == 2)
				{
					model->mesh.AddIndex((uint16_t)model->mesh.indices.size() - 1);
				}

				if (isSmoothing == true)
				{
					model->mesh.AddSmoothData((uint16_t)indexPos, (uint16_t)model->mesh.indices.size() - 1);
				}

				count++;
			}
		}
	}

	// 繝輔ぃ繧､繝ｫ繧帝哩縺倥ｋ
	file.close();

	if (isSmoothing == true)
	{
		model->mesh.CalculateSmoothedVertexNormals();
	}

	model->mesh.CreateBuffer();

	std::string log = "[ObjModel Load] FileName : " + fileName + ", Tag : " + modelTag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	// map縺ｫ譬ｼ邏・
	sModelMap.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap[modelTag].get();
}

// fbx繝輔ぃ繧､繝ｫ縺九ｉ繝｢繝・Ν繧偵Ο繝ｼ繝峨＠繝槭ャ繝励・譬ｼ邏阪☆繧・
Model* ModelManager::LoadFbxModel(const std::string fileName, const std::string modelTag)
{
	// 謗剃ｻ門宛蠕｡
	std::lock_guard<std::mutex> lock(sMtx);

	// 繝｢繝・Ν逕滓・
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();
	model->name = fileName;

	// 繝｢繝・Ν縺ｨ蜷後§蜷榊燕縺ｮ繝輔か繝ｫ繝繝ｼ縺九ｉ隱ｭ縺ｿ霎ｼ繧
	std::string path = sDirectoryPath + fileName + "/";
	std::string fbxfile = fileName + ".fbx";
	std::string fullPath = path + fbxfile;

	// 繝輔Λ繧ｰ
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;

	// 繧ｷ繝ｼ繝ｳ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	model->scene = model->importer.ReadFile(fullPath, flags);

	if (model->scene == nullptr)
	{
		std::string log = "[ObjModel Load] FileName : " + fileName + ", Tag : " + modelTag + ", is,failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "繝｢繝・Ν縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ縺悟､ｱ謨励＠縺ｾ縺励◆");
	}

	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ隗｣譫・
	AssimpLoader::GetInstance()->ParseMaterial(model.get(), model->scene);
	AssimpLoader::GetInstance()->ParseNodeRecursive(model.get(), nullptr, model->scene->mRootNode);

	// 繝舌ャ繝輔ぃ繝ｼ逕滓・
	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	// map縺ｫ譬ｼ邏・
	sModelMap.insert(std::make_pair(modelTag, std::move(model)));


	std::string log = "[FbxModel Load] FileName : " + fileName + ", Tag : " + modelTag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	return sModelMap[modelTag].get();
}

std::unordered_map<std::string, std::unique_ptr<Model>>* ModelManager::GetModelMap()
{
	return &sModelMap;
}

// .mtl繝輔ぃ繧､繝ｫ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
void ModelManager::LoadMaterialColor(std::string filePath, Model* model)
{
	// 繝輔ぃ繧､繝ｫ繧ｹ繝医Μ繝ｼ繝
	std::ifstream file;
	// .mtl繝輔ぃ繧､繝ｫ繧帝幕縺・
	file.open(filePath);
	// 繝輔ぃ繧､繝ｫ繧ｪ繝ｼ繝励Φ螟ｱ謨励ｒ繝√ぉ繝・け
	if (file.fail())
	{
		assert(0 && "繝槭ユ繝ｪ繧｢繝ｫ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ縺悟､ｱ謨励＠縺ｾ縺励◆");
	}

	// 繝・ぅ繝ｬ繧ｯ繝医Μ繝代せ
	std::string directoryPath = filePath;
	while (true)
	{
		directoryPath.pop_back();
		if (directoryPath.back() == '/')
		{
			break;
		}
	}

	// 1陦後★縺､隱ｭ縺ｿ霎ｼ繧
	std::string line;
	while (getline(file, line))
	{
		// 1陦悟・縺ｮ譁・ｭ怜・繧偵せ繝医Μ繝ｼ繝縺ｫ螟画鋤縺励※隗｣譫舌＠繧・☆縺上☆繧・
		std::istringstream lineStream(line);

		// 蜊願ｧ偵せ繝壹・繧ｹ蛹ｺ蛻・ｊ縺ｧ陦後・蜈磯ｭ譁・ｭ怜・繧貞叙蠕・
		std::string key;
		std::getline(lineStream, key, ' ');

		// 蜈磯ｭ縺ｮ繧ｿ繝匁枚蟄励ｒ辟｡隕悶☆繧・
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		// 蜈磯ｭ譁・ｭ怜・縺系ewmtl縺ｪ繧峨・繝・Μ繧｢繝ｫ蜷・
		if (key == "newmtl")
		{
			// 繝槭ユ繝ｪ繧｢繝ｫ蜷崎ｪｭ縺ｿ霎ｼ縺ｿ
			lineStream >> model->material.name;
		}

		// 蜈磯ｭ譁・ｭ怜・縺桑a縺ｪ繧峨い繝ｳ繝薙お繝ｳ繝郁牡
		if (key == "Ka")
		{
			lineStream >> model->material.ambient.r;
			lineStream >> model->material.ambient.g;
			lineStream >> model->material.ambient.b;
		}

		// 蜈磯ｭ譁・ｭ怜・縺桑a縺ｪ繧峨ョ繧｣繝輔・繧ｺ濶ｲ
		if (key == "Kd")
		{
			lineStream >> model->material.diffuse.r;
			lineStream >> model->material.diffuse.g;
			lineStream >> model->material.diffuse.b;
		}

		// 蜈磯ｭ譁・ｭ怜・縺桑a縺ｪ繧峨せ繝壹く繝･繝ｩ繝ｼ濶ｲ
		if (key == "Ks")
		{
			lineStream >> model->material.specular.r;
			lineStream >> model->material.specular.g;
			lineStream >> model->material.specular.b;
		}

		// 蜈磯ｭ譁・ｭ怜・縺稽ap_Kd縺ｪ繧峨ユ繧ｯ繧ｹ繝√Ε繝輔ぃ繧､繝ｫ蜷・
		if (key == "map_Kd")
		{
			// 繝・け繧ｹ繝√Ε縺ｮ繝輔ぃ繧､繝ｫ蜷崎ｪｭ縺ｿ霎ｼ縺ｿ
			std::string textureName;
			lineStream >> textureName;

			std::string textureTag = model->name + "Texture";

			// 繝・け繧ｹ繝√Ε隱ｭ縺ｿ霎ｼ縺ｿ
			model->texture = TextureManager::LoadMaterialTexture(directoryPath + textureName, textureTag);
		}
	}

	// 繝輔ぃ繧､繝ｫ繧帝哩縺倥ｋ
	file.close();
}

