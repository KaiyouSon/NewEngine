#pragma once
#include "Color.h"
#include "FrameRate.h"
#include "Random.h"
#include "Singleton.h"
#include "MathUtil.h"
#include "Transform.h"
#include "Camera.h"
#include "SceneManager.h"
#include <memory>
#include <vector>
#include <string>
#include <list>

enum BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv
};

// ”äŠr‚µ‚Ä‘å‚«‚¢•û‚ğ•Ô‚·
float Max(const float& a, const float& b);

// ”äŠr‚µ‚Ä¬‚³‚¢•û‚ğ•Ô‚·
float Min(const float& a, const float& b);

// •„†‚ğ•Ô‚·i -1, 0, 1 j
int Sign(const float& a);

// ’l‚ğ§ŒÀ‚·‚éŠÖ”
float Clamp(const float& value, const float& min = 0, const float& max = 1);

// Œ…”‚ğæ“¾
int GetDight(const int& value);
