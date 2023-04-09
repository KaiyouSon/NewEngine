#include "GuiManager.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "TextureManager.h"
#include "Util.h"
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

const int GuiManager::NumFramesInFlight = 3;

void GuiManager::Init()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(RenderWindow::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		renderBase->GetDevice(),
		NumFramesInFlight,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		TextureManager::GetSrvDescHeap(),
		TextureManager::GetSrvDescHeap()->GetCPUDescriptorHandleForHeapStart(),
		TextureManager::GetSrvDescHeap()->GetGPUDescriptorHandleForHeapStart());
}

void GuiManager::PreDraw()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::NewFrame();
}

void GuiManager::PostDraw()
{
	ImGui::Render();
	// SRV�q�[�v�̐ݒ�R�}���h
	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	auto srvDescHeap = TextureManager::GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &srvDescHeap);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderBase->GetCommandList());
}

void GuiManager::Destroy()
{
	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool GuiManager::BeginWindow(const char* name, const Vec2& size)
{
	if (size.x != -1 && size.y != -1)
	{
		ImGui::SetNextWindowSize({ size.x,size.y });
		return ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoResize);
	}

	return ImGui::Begin(name);
}

void GuiManager::BeginFullWindow(const char* name)
{
	// �E�B���h�E�̐ݒ�
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoTitleBar |				// �^�C�g���o�[�\�����Ȃ�
		ImGuiWindowFlags_NoResize |					// �T�C�Y�ύX���Ȃ�
		ImGuiWindowFlags_NoMove |					// ���W�ύX���Ȃ�
		ImGuiWindowFlags_MenuBar |					// ���j���[�o�[�\��
		ImGuiWindowFlags_NoBringToFrontOnFocus |	// �N���b�N������őO�ʂɕ\�����Ȃ�
		ImGuiWindowFlags_NoCollapse;				// �܂��݂��Ȃ�

	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
	{
		windowFlags |= ImGuiWindowFlags_NoBackground;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(name, nullptr, windowFlags);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}
}

void GuiManager::EndFullWindow()
{
	ImGui::End();
	ImGui::PopStyleVar();
}

void GuiManager::OpenPopModal(const char* tag)
{
	ImGui::OpenPopup(tag);
}

void GuiManager::ClosePopModal()
{
	ImGui::CloseCurrentPopup();
}

bool GuiManager::BeginPopModal(const char* tag)
{
	return ImGui::BeginPopupModal(tag);
}

void GuiManager::EndPopModal()
{
	ImGui::EndPopup();
}

void GuiManager::EndWindow()
{
	ImGui::End();
}

void GuiManager::DrawDemoWindow(bool& flag)
{
	ImGui::ShowDemoWindow(&flag);
}

bool GuiManager::DrawButton(const char* label, const Vec2& size)
{
	return ImGui::Button(label, { size.x,size.y });
}

void GuiManager::DrawTab()
{
	ImGui::SameLine();
}

void GuiManager::DrawColumns(int space, const bool& isBorder)
{
	ImGui::Columns(space, 0, isBorder);
}

void GuiManager::NextColumn()
{
	ImGui::NextColumn();
}

void GuiManager::DrawLine()
{
	ImGui::Separator();
}

void GuiManager::DrawString(const char* fmt, ...)
{
	ImGui::Text(fmt);
}

void GuiManager::DrawCheckBox(const char* label, bool* flag)
{
	ImGui::Checkbox(label, flag);
}

bool GuiManager::DrawRadioButton(const char* label, int& current, const int& index, const bool& isTab)
{
	bool flag = ImGui::RadioButton(label, &current, index);
	if (isTab == true)
	{
		ImGui::SameLine();
	}

	return flag;
}

void GuiManager::DrawSlider1(const char* label, float& v, const float& moveSpeed)
{
	ImGui::DragFloat(label, &v, moveSpeed);
}

void GuiManager::DrawSlider2(const char* label, Vec2& v, const float& moveSpeed)
{
	float temp[2] = { v.x,v.y };
	ImGui::DragFloat2(label, temp, moveSpeed);
	v.x = temp[0]; v.y = temp[1];
}

void GuiManager::DrawSlider3(const char* label, Vec3& v, const float& moveSpeed)
{
	float temp[3] = { v.x,v.y,v.z };
	ImGui::DragFloat3(label, temp, moveSpeed);
	v.x = temp[0];	v.y = temp[1];	v.z = temp[2];
}

void GuiManager::DrawColorEdit(const char* label, Color& color)
{
	float temp[4] = { color.r / 255,color.g / 255,color.b / 255,color.a / 255 };
	ImGui::ColorEdit4(label, temp);
	color.r = temp[0] * 255; color.g = temp[1] * 255; color.b = temp[2] * 255; color.a = temp[3] * 255;
}

bool GuiManager::DrawInputInt(const char* label, int& v)
{
	bool flag = ImGui::InputInt(label, &v);
	return flag;
}

bool GuiManager::DrawInputText(const char* label, const std::string& str)
{
	return ImGui::InputText(label, const_cast<char*>(str.c_str()), 30);
}

void GuiManager::DrawImage(Texture* texture, const Vec2& size)
{
	if (texture == nullptr) return;

	ImTextureID gpuHandle = (ImTextureID)texture->GetGpuHandle().ptr;
	ImVec2 textureSize = { size.x,size.y };
	ImGui::Image(gpuHandle, textureSize);
}

bool GuiManager::DrawImageButton(Texture* texture, const Vec2& size)
{
	if (texture == nullptr) return false;

	ImTextureID gpuHandle = (ImTextureID)texture->GetGpuHandle().ptr;
	ImVec2 buttonSize = { size.x,size.y };
	return ImGui::ImageButton(gpuHandle, buttonSize);
}
