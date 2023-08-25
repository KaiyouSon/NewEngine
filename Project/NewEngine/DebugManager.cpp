#include "DebugManager.h"
#include "NewEngine.h"

void DebugManager::Init()
{
	mIsActive = true;

	mIsShowLoadedWindow = false;
}

void DebugManager::Update()
{
	if (Key::GetKeyDown(DIK_F5))
	{
		if (mIsActive == false)
		{
			mIsActive = true;
		}
		else
		{
			mIsActive = false;
		}
	}
}

void DebugManager::DrawDebugGui()
{
	if (mIsActive == false)
	{
		return;
	}

	Gui::BeginWindow("Debug Manager");

	if (Gui::BeginMenuBar())
	{
		if (Gui::BeginMenu("Display"))
		{
			Gui::BeginMenuBar();

			if (Gui::MenuItem("Loaded")) mIsShowLoadedWindow = true;

			Gui::EndMenu();
		}
		Gui::EndMenuBar();
	}

	// ストップする
	Gui::DrawCheckBox("Stop", &mIsStop);
	Gui::DrawTab();
	if (Gui::DrawButton("Next Frame", Vec2(96, 24)))
	{
		mIsNextFrame = true;
	}
	Gui::DrawLine();

	// 当たり判定の表示
	bool isShowCollider = ColliderDrawer::GetInstance()->GetisShow();
	Gui::DrawCheckBox("Show Collider", &isShowCollider);
	ColliderDrawer::GetInstance()->SetisShow(isShowCollider);

	Gui::EndWindow();

	LoadedWindow(&mIsShowLoadedWindow);

	static bool isActive = true;
	Gui::DrawDemoWindow(isActive);
}

bool DebugManager::GetisStop()
{
	return mIsStop;
}

bool DebugManager::GetisNextFrame()
{
	return mIsNextFrame;
}

void DebugManager::SetisNextFrame(const bool isNextFrame)
{
	mIsNextFrame = isNextFrame;
}

void DebugManager::LoadedWindow(bool* flag)
{
	if (mIsShowLoadedWindow == false)
	{
		return;
	}

	Gui::BeginWindow("Loaded Window", -1, flag);

	Gui::DrawString("std::unordered_map<std::string, std::unique_ptr<Texture>>");

	for (const auto& pair : *TextureManager::GetTextureMap())
	{
		if (Gui::DrawCollapsingHeader(pair.first.c_str()))
		{
			Gui::DrawString("Texture Size : (%f,%f)", pair.second->size.x, pair.second->size.y);
			Gui::DrawImage(pair.second.get(), pair.second->size);

		}
		Gui::DrawLine();
	}

	Gui::EndWindow();
}
