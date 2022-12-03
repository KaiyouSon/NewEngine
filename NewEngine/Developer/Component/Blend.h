#pragma once
#include "IComponent.h"

enum BlendMode
{
	Alpha3D,
	Add3D,
	Alpha2D,
	AlphaLine,
	AlphaRenderTexture,
	AlphaObject,
	ParticleAlpha
};

class Blend : public IComponent
{
public:
	void SetBlendMode(const int& blendmode);
};
