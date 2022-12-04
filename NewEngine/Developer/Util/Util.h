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

class Util
{
public:
	// ”äŠr‚µ‚Ä‘å‚«‚¢•û‚ğ•Ô‚·
	static float Max(float a, float b);

	// ”äŠr‚µ‚Ä¬‚³‚¢•û‚ğ•Ô‚·
	static float Min(float a, float b);

	// •„†‚ğ•Ô‚·i -1, 0, 1 j
	static int Sign(float a);
};