#include "DebugManager.h"
#include "NewEngine.h"

DebugManager::DebugManager() :
	mShaderWindow(std::make_unique<ShaderWindow>()),
	mMappingWindow(std::make_unique<MappingWindow>()),
	mRendererWindow(std::make_unique<RendererWindow>()),
	mPipelineWindow(std::make_unique<PipelineWindow>())
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
	//mPipelineWindow->DrawDebugGui();

	if (mIsActive == false)
	{
		return;
	}

	Gui::BeginWindow("Debug Manager");

	// メニューバー
	if (Gui::BeginMenuBar())
	{
		if (Gui::BeginMenu("Display"))
		{
			Gui::BeginMenuBar();

			if (Gui::MenuItem("Shader"))
			{
				mShaderWindow->SetisShow(true);
			}
			else if (Gui::MenuItem("Map Data"))
			{
				mMappingWindow->SetisShow(true);
			}
			else if (Gui::MenuItem("Renderer"))
			{
				mRendererWindow->SetisShow(true);
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

	Gui::EndWindow();

	//ImGui::ShowDemoWindow();

	mShaderWindow->DrawDebugGui();
	mMappingWindow->DrawDebugGui();
	mRendererWindow->DrawDebugGui();
}

void DebugManager::ReCompile()
{
	mShaderWindow->ReCompile();
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

