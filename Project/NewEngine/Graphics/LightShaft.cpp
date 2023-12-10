#include "LightShaft.h"
#include "PostEffect.h"

LightShaft::LightShaft() :
	mPostEffect(std::make_unique<PostEffect>())
{
}

void LightShaft::Update()
{
	mPostEffect->Update();
}

void LightShaft::TransferData()
{
}

void LightShaft::PrevSceneDraw()
{
	mRenderTexture->PrevDrawScene();
}

void LightShaft::PostSceneDraw()
{
	mRenderTexture->PostDrawScene();
}

void LightShaft::DrawCommands()
{
	mPostEffect->Draw();
}
