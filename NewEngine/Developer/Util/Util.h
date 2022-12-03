#pragma once
#include "Color.h"
#include "Random.h"
#include "Singleton.h"
#include "FrameRate.h"
#include <memory>
#include <vector>
#include <string>
#include <list>

const int WIN_WIDTH = 1920;
const int WIN_HEIGHT = 1010;
const int WIN_HALF_WIDTH = WIN_WIDTH / 2;
const int WIN_HALF_HEIGHT = WIN_HEIGHT / 2;

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