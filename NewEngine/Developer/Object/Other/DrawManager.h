#pragma once
#include "Object3D.h"
#include "Line.h"
#include "Emitter.h"
#include "RenderTexture.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "DebugCamera.h"
#include "TextureBuffer.h"
#include "ComponentManager.h"

#include <memory>

// テクスチャーの読み込み
std::unique_ptr<Texture> LoadTexture(const char* filePath);
// モデルの読み込み
std::unique_ptr<ModelData> LoadModelData(const char* filePath);
// マテリアルの世も込み
std::unique_ptr<Material> LoadMaterial(const std::string& filePath);

void UnLoadTexture(Texture* texture);
