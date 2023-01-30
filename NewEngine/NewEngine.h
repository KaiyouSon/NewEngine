#pragma once
// 自作
#include "Util.h"
#include "Object3D.h"
#include "SilhouetteObj.h"
#include "OutLineObj.h"
#include "Fog.h"
#include "Sprite.h"
#include "Sound.h"
#include "imgui.h"
#include "IScene.h"
#include "GuiManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "LightManager.h"
#include "Collision.h"

// c++
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <array>
#include <map>

void NewEngineInit();
void NewEngineUpda();
void NewEnginePreDraw();
void NewEneineDraw();
void NewEnginePostDraw();
void NewEngineEnd();

// ウインドウ関連
bool ProcessMessage();
void SetWindowTitle(const std::string& title);
void SetWindowSize(const Vec2& size);
void SetBackGroundColor(const float& r, const float& g, const float& b);
Vec2 GetWindowSize();
Vec2 GetWindowHalfSize();

// FPS関連
void SetFrameRate(const float& frameRate);
void FrameRateUpdate();
