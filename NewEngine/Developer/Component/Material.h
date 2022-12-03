#pragma once
#include "MathUtil.h"
#include "Texture.h"
#include <string>
#include <memory>
#include <list>

class Material
{
private:
	uint32_t index;	// �^�O�i���f���f�[�^�ƈ�v�ɂ��Ƃ��j

public:
	std::string name;	// �}�e���A����
	Vec3 ambient;		// �A���r�G���g
	Vec3 diffuse;		// �f�B�t���[�Y
	Vec3 specular;		// �X�y�L�����[
	float alpha;		// �A���t�@
	std::string textureFilepath;	// �e�N�X�`���[�̃t�@�C���p�X

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