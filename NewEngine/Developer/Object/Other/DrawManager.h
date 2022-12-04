#pragma once
#include "Sprite.h"
#include "Object3D.h"
#include "RenderTexture.h"
#include "Line.h"
#include "DebugCamera.h"
#include "TextureBuffer.h"

#include <memory>

// �e�N�X�`���[�̓ǂݍ���
std::unique_ptr<Texture> LoadTexture(const char* filePath);
// ���f���̓ǂݍ���
std::unique_ptr<Model> LoadModelData(const char* filePath);
// �}�e���A���̐�������
std::unique_ptr<Material> LoadMaterial(const std::string& filePath);

void UnLoadTexture(Texture* texture);
