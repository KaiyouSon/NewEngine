#pragma once
#include "MathUtil.h"
#include "Texture.h"
#include <string>
#include <memory>
#include <list>

class Material
{
private:
	uint32_t index;	// タグ（モデルデータと一致にしとく）

public:
	std::string name;	// マテリアル名
	Vec3 ambient;		// アンビエント
	Vec3 diffuse;		// ディフューズ
	Vec3 specular;		// スペキュラー
	float alpha;		// アルファ
	std::string textureFilepath;	// テクスチャーのファイルパス

public:
	Material();
	inline void SetMaterial(const Material& material) { *this = material; }
	inline void SetIndex(const uint32_t& index) { this->index = index; }

	inline uint32_t GetIndex() { return index; }
};

class MaterialManager
{
public:
	std::list<std::unique_ptr<Material>> list;

public:
	void AddMaterial(std::unique_ptr<Material>&& material, const uint32_t& index);

	Material* GetMaterial(const uint32_t& index);
	void DestroyMaterial(const uint32_t& index);
};

extern std::unique_ptr<MaterialManager> materialList;