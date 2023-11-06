#include "DebugManager.h"
#include "NewEngine.h"

DebugManager::DebugManager() :
	mMappingWindow(std::make_unique<MappingWindow>())
{

}

void DebugManager::Init()
{
	mIsActive = false;
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

	Gui::DrawCollapsingHeader("Current Camera");
	Gui::DrawSlider3("Pos", Camera::current.pos);
	Vec3 angle = Angle(Camera::current.rot);
	Gui::DrawSlider3("Rot", angle);
	Camera::current.rot = Radian(angle);

	Gui::DrawSlider1("Far Z", Camera::current.oFarZ);
	Gui::DrawSlider1("Near Z", Camera::current.oNearZ);
	Gui::DrawSlider1("Fov Angle", Camera::current.fov);
	Gui::DrawLine();

	// 処理を止める
	Gui::DrawCheckBox("Stop", &mIsStop);
	Gui::DrawTab();
	if (Gui::DrawButton("Next Frame", Vec2(96, 24)))
	{
		mIsNextFrame = true;
	}
	Gui::DrawLine();

	// あたり判定表示
	bool isShowCollider = ColliderDrawer::GetInstance()->GetisShow();
	Gui::DrawCheckBox("Show Collider", &isShowCollider);
	ColliderDrawer::GetInstance()->SetisShow(isShowCollider);

	Gui::DrawLine();
	Gui::DrawString("FPS : %f", FrameRate::GetInstance()->GetCurrentFPS());

	Gui::EndWindow();

	mMappingWindow->DrawDebugGUI();
}

bool DebugManager::GetisStop()
{
	return mIsStop;
}

bool DebugManager::GetisActive()
{
	return mIsActive;
}

bool DebugManager::GetisNextFrame()
{
	return mIsNextFrame;
}

void DebugManager::SetisNextFrame(const bool isNextFrame)
{
	mIsNextFrame = isNextFrame;
}

