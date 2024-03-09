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

	Gui::BeginWindow("Scene");
	Vec2 windowSize = Gui::ToVec2(ImGui::GetContentRegionAvail());

	Vec2 maxSize =
	{
		windowSize.y * mAspect.x,
		windowSize.x * mAspect.y,
	};
	Vec2 size =
	{
		Min(windowSize.x, maxSize.x),
		Min(windowSize.y, maxSize.y)
	};

	Gui::DrawImage(TextureManager::GetRenderTexture("Scene"), size);
	Gui::EndWindow();
}
