#include "DebugManager.h"
#include "NewEngine.h"

DebugManager::DebugManager() :
	mMappingWindow(std::make_unique<MappingWindow>())
{

}

void DebugManager::Init()
{
	mIsActive = true;
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

			if (Gui::MenuItem("All Map"))
			{
				mMappingWindow->SetisShow(true);
			}

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

	mMappingWindow->DrawDebugGUI();

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


