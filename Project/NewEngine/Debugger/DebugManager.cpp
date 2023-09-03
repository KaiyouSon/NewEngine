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

	// �X�g�b�v����
	Gui::DrawCheckBox("Stop", &mIsStop);
	Gui::DrawTab();
	if (Gui::DrawButton("Next Frame", Vec2(96, 24)))
	{
		mIsNextFrame = true;
	}
	Gui::DrawLine();

	// �����蔻��̕\��
	bool isShowCollider = ColliderDrawer::GetInstance()->GetisShow();
	Gui::DrawCheckBox("Show Collider", &isShowCollider);
	ColliderDrawer::GetInstance()->SetisShow(isShowCollider);

	Gui::EndWindow();

	mMappingWindow->DrawDebugGUI();

	//static bool isActive = true;
	//Gui::DrawDemoWindow(isActive);
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

