#include "MappingWindow.h"
#include "GuiManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "SoundManager.h"

MappingWindow::MappingWindow() :
	mIsShow(false), currentType(None)
{
}

void MappingWindow::DrawDebugGUI()
{
	if (mIsShow == false)
	{
		return;
	}

	Gui::BeginWindow("Loaded Window", -1, &mIsShow);

	Gui::DrawRadioButton("None", &currentType, None);
	Gui::DrawRadioButton("Texture Map", &currentType, Texture);
	Gui::DrawRadioButton("Model Map", &currentType, Model);
	Gui::DrawRadioButton("Sound Map", &currentType, Sound);
	Gui::DrawLine();

	switch (currentType)
	{
	case Texture:
		ShowTextureMap();
		break;

	case Model:
		ShowModelMap();
		break;

	case Sound:
		ShowSoundMap();
		break;
	}

	Gui::EndWindow();
}

void MappingWindow::ShowTextureMap()
{
	Gui::DrawString("Texture Map");
	Gui::DrawLine();

	for (const auto& pair : *TextureManager::GetTextureMap())
	{
		if (Gui::DrawCollapsingHeader(pair.first.c_str()))
		{
			Gui::DrawString("Texture Size : (%f,%f)", pair.second->size.x, pair.second->size.y);
			Gui::DrawImage(pair.second.get(), pair.second->size);

		}
		Gui::DrawLine();
	}
}

void MappingWindow::ShowModelMap()
{
	Gui::DrawString("Model Map");
	Gui::DrawLine();

	for (const auto& pair : *ModelManager::GetModelMap())
	{
		if (Gui::DrawCollapsingHeader(pair.first.c_str()))
		{
			switch (pair.second->format)
			{
			case ModelFormat::Obj:
				Gui::DrawString("Model Format : Obj");
				break;
			case ModelFormat::Fbx:
				Gui::DrawString("Model Format : Fbx");
				break;
			}
		}
		Gui::DrawLine();
	}
}

void MappingWindow::ShowSoundMap()
{
	Gui::DrawString("Sound Map");
	Gui::DrawLine();

	for (const auto& pair : *SoundManager::GetSoundMap())
	{
		if (Gui::DrawCollapsingHeader(pair.first.c_str()))
		{
			if (Gui::DrawButton("Play", Vec2(96, 24)))
			{
				pair.second->Play();
			}

			Gui::DrawTab();

			if (Gui::DrawButton("Stop", Vec2(96, 24)))
			{
				if (pair.second->GetIsPlaying() == true)
				{
					pair.second->Stop();
				}
			}
		}
		Gui::DrawLine();
	}
}

void MappingWindow::SetisShow(const bool isShow)
{
	mIsShow = isShow;
}

bool MappingWindow::GetisShow()
{
	return mIsShow;
}