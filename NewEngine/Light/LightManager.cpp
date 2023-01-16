#include "LightManager.h"

const int LightManager::directionalLightNum = 3;
const int LightManager::pointLightNum = 3;
const int LightManager::spotLightNum = 3;

void LightManager::Init()
{
	for (int i = 0; i < directionalLights.size(); i++)
	{
		directionalLights[i].Init();
	}

	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i].Init();
	}
}

void LightManager::Update()
{
	for (int i = 0; i < directionalLights.size(); i++)
	{
		directionalLights[i].Update();
	}

	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i].Update();
	}

	for (int i = 0; i < spotLights.size(); i++)
	{
		spotLights[i].Update();
	}

	circleShadow.Update();
}

void LightManager::Draw()
{
	for (int i = 0; i < directionalLights.size(); i++)
	{
		directionalLights[i].Draw();
	}

	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i].Draw();
	}

	for (int i = 0; i < spotLights.size(); i++)
	{
		spotLights[i].Draw();
	}

	circleShadow.Draw();
}
