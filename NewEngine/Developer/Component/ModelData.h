#pragma once
#include "IComponent.h"
#include "Material.h"
#include "VertexBuffer.h"
#include <vector>
#include <list>
#include <string>
#include <memory>

enum DataType
{
	DefaultModel,
	LoadModel,
};

struct ModelData : public IComponent
{
private:
	int dataType;
	std::string tag;
	std::string filePath;
	int index = -1;

public:
	std::vector<VertexPosNormalUv> vertices;
	std::vector<unsigned short> indices;

public: // セッター
	inline void SetDataType(const int& dataType) { this->dataType = dataType; }
	inline void SetTag(const std::string& tag) { this->tag = tag; }
	inline void SetFilePath(const std::string& filePath) { this->filePath = filePath; }
	inline void SetIndex(const int& index) { this->index = index; }
	inline void SetModelData(const ModelData& modelData)
	{
		//*this = modelData;

		dataType = modelData.dataType;
		tag = modelData.tag;
		filePath = modelData.filePath;
		vertices = modelData.vertices;
		indices = modelData.indices;
		index = modelData.index;
	}

public: // ゲッター
	inline int GetDataType() { return dataType; }
	inline std::string GetTag() { return tag; }
	inline std::string GetFilePath() { return filePath; }
	inline int GetIndex() { return index; }
};

class ModelDataManager
{
public:
	std::list<std::unique_ptr<ModelData>> list;

public:
	ModelDataManager();
	void Add(std::unique_ptr<ModelData>&& modelData, const std::string& tag);

public: // ゲッター
	//const std::unique_ptr<ModelData>& GetModelData(const std::string& tag);
	ModelData* GetModelData(const std::string& tag);
	void DestroyModelData(const std::string& tag);
};

extern std::unique_ptr<ModelDataManager> modelDataList;