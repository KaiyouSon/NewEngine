#pragma once
// 自作
#include "Util.h"
#include "Object3D.h"
#include "Sprite.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Sound.h"
#include "imgui.h"
#include "GuiManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Collision.h"

// c++
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <array>

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
