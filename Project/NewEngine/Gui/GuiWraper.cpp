#include "GuiWraper.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "TextureManager.h"
#include "Util.h"
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

GuiWraper::GuiWraper() :
	isOpenGuiSetting(true),
	windowBgColor(Color::black + 5),
	menuBarBgColor(Color::black + 10),
	titleBgActiveColor(Color::black),
	titleBgCollapsedColor(Color::black + 5)
{
}

ImVec4 GuiWraper::ToImVec4(const Color color)
{
	Color to01 = color / 255.f;
	return { to01.r,to01.g,to01.b,to01.a };
}

void GuiWraper::Init()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(RenderWindow::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		renderBase->GetDevice(),
		sNumFramesInFlight,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap(),
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
}

void GuiWraper::PreDraw()
{
	GuiWraper* instance = GetInstance().get();

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// ウィンドウの背景色を変更する
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = GetInstance()->ToImVec4(instance->windowBgColor);
	style.Colors[ImGuiCol_MenuBarBg] = GetInstance()->ToImVec4(instance->menuBarBgColor);
	style.Colors[ImGuiCol_TitleBgActive] = GetInstance()->ToImVec4(instance->titleBgActiveColor);
	style.Colors[ImGuiCol_TitleBgCollapsed] = GetInstance()->ToImVec4(instance->titleBgCollapsedColor);

	ImGui::NewFrame();

	DrawGuiSetting();
}

void GuiWraper::PostDraw()
{
	ImGui::Render();
	// SRVヒープセット
	RenderBase* renderBase = RenderBase::GetInstance();
	auto srvDescHeap = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &srvDescHeap);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderBase->GetCommandList());
}

void GuiWraper::Destroy()
{
	// 破棄
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GuiWraper::DrawGuiSetting()
{
	//GuiWraper* instance = GetInstance().get();
	//BeginWindow("Gui Setting", -1, &instance->isOpenGuiSetting);
	//DrawColorEdit("Window BG Color", instance->windowBgColor);
	//DrawColorEdit("MenuBar BG Color", instance->menuBarBgColor);
	//DrawColorEdit("Title BG Active Color", instance->titleBgActiveColor);
	//DrawColorEdit("Title BG Collapsed Color", instance->titleBgCollapsedColor);
	//EndWindow();
}

bool GuiWraper::BeginWindow(const char* name, const Vec2& size, bool* isOpen)
{
	if (size.x != -1 && size.y != -1)
	{
		ImGui::SetNextWindowSize({ size.x,size.y });

		ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoResize;
		return ImGui::Begin(name, isOpen, windowFlags);
	}

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
	return ImGui::Begin(name, isOpen, windowFlags);
}

void GuiWraper::BeginFullWindow(const char* name)
{
	// 繧ｦ繧｣繝ｳ繝峨え縺ｮ險ｭ螳・
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoTitleBar |                // タイトルバーを非表示にする
		ImGuiWindowFlags_NoResize |                 // ウィンドウのリサイズを禁止する
		ImGuiWindowFlags_NoMove |                   // ウィンドウの移動を禁止する
		ImGuiWindowFlags_MenuBar |                  // メニューバーを表示する
		ImGuiWindowFlags_NoBringToFrontOnFocus |    // フォーカス時にウィンドウを前面に持ってこない
		ImGuiWindowFlags_NoCollapse;                // ウィンドウの折り畳みを禁止する

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

void GuiWraper::EndFullWindow()
{
	ImGui::End();
	ImGui::PopStyleVar();
}

void GuiWraper::OpenPopModal(const char* tag)
{
	ImGui::OpenPopup(tag);
}

void GuiWraper::ClosePopModal()
{
	ImGui::CloseCurrentPopup();
}

bool GuiWraper::BeginPopModal(const char* tag)
{
	return ImGui::BeginPopupModal(tag);
}

void GuiWraper::EndPopModal()
{
	ImGui::EndPopup();
}

bool GuiWraper::BeginTreeNode(const std::string& tag, const bool isOpenNode)
{
	uint32_t nodeFlag =
		(isOpenNode == false) ?
		ImGuiTreeNodeFlags_None :
		ImGuiTreeNodeFlags_DefaultOpen;

	return ImGui::TreeNodeEx(tag.c_str(), nodeFlag);
}

void GuiWraper::EndTreeNode()
{
	ImGui::TreePop();
}

bool GuiWraper::DrawCollapsingHeader(const char* name)
{
	return ImGui::CollapsingHeader(name);
}

bool GuiWraper::BeginMenuBar()
{
	return ImGui::BeginMenuBar();
}

void GuiWraper::EndMenuBar()
{
	ImGui::EndMenuBar();
}

bool GuiWraper::BeginMenu(const char* name)
{
	return ImGui::BeginMenu(name);
}

void GuiWraper::EndMenu()
{
	ImGui::EndMenu();
}

bool GuiWraper::MenuItem(const char* name)
{
	return ImGui::MenuItem(name);
}

void GuiWraper::EndWindow()
{
	ImGui::End();
}

void GuiWraper::DrawDemoWindow(bool& flag)
{
	ImGui::ShowDemoWindow(&flag);
}

bool GuiWraper::DrawButton(const char* label, const Vec2& size)
{
	return ImGui::Button(label, { size.x,size.y });
}

void GuiWraper::DrawTab()
{
	ImGui::SameLine();
}

void GuiWraper::DrawColumns(uint32_t space, const bool& isBorder)
{
	ImGui::Columns(space, 0, isBorder);
}

void GuiWraper::NextColumn()
{
	ImGui::NextColumn();
}

void GuiWraper::DrawLine()
{
	ImGui::Separator();
}

void GuiWraper::DrawString(const char* fmt, ...)
{
	ImGui::Text(fmt);
}

void GuiWraper::DrawCheckBox(const char* label, bool* flag)
{
	ImGui::Checkbox(label, flag);
}

bool GuiWraper::DrawRadioButton(const char* label, uint32_t* current, const uint32_t index, const bool isTab)
{
	bool flag = ImGui::RadioButton(label, (int*)current, (int)index);
	if (isTab == true)
	{
		ImGui::SameLine();
	}

	return flag;
}

void GuiWraper::DrawSlider1(const char* label, float& v, const float& moveSpeed)
{
	ImGui::DragFloat(label, &v, moveSpeed);
}

void GuiWraper::DrawSlider2(const char* label, Vec2& v, const float& moveSpeed)
{
	float temp[2] = { v.x,v.y };
	ImGui::DragFloat2(label, temp, moveSpeed);
	v.x = temp[0]; v.y = temp[1];
}

void GuiWraper::DrawSlider3(const char* label, Vec3& v, const float& moveSpeed)
{
	float temp[3] = { v.x,v.y,v.z };
	ImGui::DragFloat3(label, temp, moveSpeed);
	v.x = temp[0];	v.y = temp[1];	v.z = temp[2];
}

void GuiWraper::DrawColorEdit(const char* label, Color& color)
{
	float temp[4] = { color.r / 255,color.g / 255,color.b / 255,color.a / 255 };
	ImGui::ColorEdit4(label, temp);
	color.r = temp[0] * 255; color.g = temp[1] * 255; color.b = temp[2] * 255; color.a = temp[3] * 255;
}

bool GuiWraper::DrawInputInt(const char* label, int32_t& v)
{
	return ImGui::InputInt(label, &v);
}

bool GuiWraper::DrawInputInt(const char* label, uint32_t& v)
{
	int intv = v;
	bool result = ImGui::InputInt(label, &intv);
	v = Max(0, intv);
	return result;
}

bool GuiWraper::DrawInputText(const char* label, std::string& str)
{
	char* cstr = const_cast<char*>(str.c_str());
	bool result = ImGui::InputText(label, cstr, 30);
	str = cstr;

	return result;
}

void GuiWraper::DrawImage(ITexture* texture, const Vec2& size)
{
	if (texture == nullptr) return;

	if (texture->GetTextureType() != TextureType::Render)
	{
		ImTextureID gpuHandle = (ImTextureID)texture->GetBufferResource()->srvHandle.gpu.ptr;
		ImVec2 textureSize = { size.x,size.y };
		ImGui::Image(gpuHandle, textureSize);
	}
	else
	{
		RenderTexture* renderTex = dynamic_cast<RenderTexture*>(texture);
		for (uint32_t i = 0; i < renderTex->GetBufferResources()->size(); i++)
		{
			ImTextureID gpuHandle = (ImTextureID)renderTex->GetBufferResources()->at(i).srvHandle.gpu.ptr;
			ImVec2 textureSize = { size.x,size.y };
			ImGui::Image(gpuHandle, textureSize);
		}
	}
}

void GuiWraper::DrawImage(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, const Vec2& size)
{
	ImTextureID handle = (ImTextureID)gpuHandle.ptr;
	ImVec2 textureSize = { size.x,size.y };
	ImGui::Image(handle, textureSize);
}

bool GuiWraper::DrawImageButton(ITexture* texture, const Vec2& size)
{
	if (texture == nullptr) return false;

	ImTextureID gpuHandle = (ImTextureID)texture->GetBufferResource()->srvHandle.gpu.ptr;
	ImVec2 buttonSize = { size.x,size.y };
	return ImGui::ImageButton(gpuHandle, buttonSize);
}


