#include "NewEngine/Header/Gui/SceneLayer.h"
#include "NewEngine/Header/Gui/ProjectLayer.h"
#include "RenderTexture.h"

void SceneLayer::Initialize()
{
	size = { 960,540 };
	pos = {
		WIN_HALF_WIDTH - size.x / 2,
		ProjectLayer::GetInstance()->GetPos().y - size.y };
}
void SceneLayer::Update()
{
	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin("SceneView", nullptr, windowFlags);
	ImGui::Image((ImTextureID)sceneViewTexture->GetRenderTexture()->GetGpuHandle().ptr, { 960,540 });

	pos = { ImGui::GetWindowPos().x,ImGui::GetWindowPos().y };
	size = { ImGui::GetWindowSize().x,ImGui::GetWindowSize().y };
	ImGui::End();
}