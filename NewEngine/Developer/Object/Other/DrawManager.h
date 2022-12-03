#pragma once
#include "Sprite.h"
#include "Object3D.h"
#include "RenderTexture.h"
#include "Line.h"
#include "ViewProjection.h"
#include "DebugCamera.h"
#include "TextureBuffer.h"
#include "ComponentManager.h"

#include <memory>

// テクスチャーの読み込み
std::unique_ptr<Texture> LoadTexture(const char* filePath);
// モデルの読み込み
std::unique_ptr<Model> LoadModelData(const char* filePath);
// マテリアルの世も込み
std::unique_ptr<Material> LoadMaterial(const std::string& filePath);

void UnLoadTexture(Texture* texture);
