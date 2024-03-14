#include "SceneWindow.h"

SceneWindow::SceneWindow() :
	mAspect(16.f / 9.f, 9.f / 16.f)
{
}

void SceneWindow::DrawGuiWindow()
{
	mAspect =
	{
		GetWindowSize().x / GetWindowSize().y,
		GetWindowSize().y / GetWindowSize().x
	};

	Gui::BeginWindow("Scene", -1, nullptr, false);
	if (Gui::BeginMenuBar())
	{

		Gui::EndMenuBar();
	}

	// 中心の描画する
	const Vec2 buttonSize = 24;
	const Vec2 windowSize = Gui::ToVec2(ImGui::GetContentRegionAvail());

	DrawPlayButton(buttonSize, windowSize);
	Gui::DrawLine();

	Vec2 maxSize =
	{
		(windowSize.y - buttonSize.y - 16) * mAspect.x,
		windowSize.x * mAspect.y,
	};
	Vec2 size =
	{
		Min(windowSize.x , maxSize.x),
		Min(windowSize.y - buttonSize.y - 16 , maxSize.y)
	};

	Gui::DrawImage(TextureManager::GetRenderTexture("Scene"), size);
	Gui::EndWindow();
}

void SceneWindow::DrawPlayButton(const Vec2 buttonSize, const Vec2 windowSize)
{
	bool isSceneActive = gCurrentScene->GetisActive();
	Color buttonColor = Color();
	if (isSceneActive)
	{
		buttonColor = Color(0x12324d);
	}
	else
	{
		buttonColor = Color::black + 10;
	}

	ImGui::PushStyleColor(ImGuiCol_Button, Gui::ToImVec4(buttonColor));
	ImGui::SetCursorPosX(windowSize.x / 2 - buttonSize.x / 2);
	if (Gui::DrawImageButton(NewEngine::mTextureManager.GetTexture("PlayUI"), buttonSize))
	{
		bool flag = isSceneActive ? false : true;
		gCurrentScene->SetisActive(flag);
		gCurrentScene->GetGameObjectManager()->InitScriptComponent();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Play");
	}
	ImGui::PopStyleColor();
}