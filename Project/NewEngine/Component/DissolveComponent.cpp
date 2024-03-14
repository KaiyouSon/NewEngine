#include "DissolveComponent.h"
#include "MainWindow.h"

DissolveComponent::DissolveComponent()
{
	mTexture = TextureManager::GetTexture("DissolveTexture");
	mComponentInfo.type = ComponentType::DissolveComponent;
}

DissolveComponent::DissolveComponent(GameObject* gameObj) :
	Component(gameObj), mConstantBuffer(std::make_unique<ConstantBuffer<CBData>>())
{
	mTexture = TextureManager::GetTexture("DissolveTexture");
	mComponentInfo.type = ComponentType::DissolveComponent;

	mConstantBuffer->Create();
}

void DissolveComponent::Update()
{
}

void DissolveComponent::CopyComponent(Component* component)
{
	component;
}

void DissolveComponent::TransferData()
{
	if (!mConstantBuffer)
	{
		return;
	}

	// ディゾルブ
	CBData dissolveData = { dissolveColor.To01(), dissolve, colorPower };

	// 転送
	TransferDataToConstantBuffer(mConstantBuffer.get(), dissolveData);
}

void DissolveComponent::DrawCommands(const uint32_t& index)
{
	if (!mConstantBuffer)
	{
		return;
	}

	auto cmdList = RenderBase::GetInstance()->GetCommandList();

	// CBVの設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(
		index, mConstantBuffer->bufferResource->buffer->GetGPUVirtualAddress());
}

nlohmann::json DissolveComponent::SaveToJson()
{
	//nlohmann::json DissolveComponent;
	//DissolveComponent["material_component"] = mComponentInfo.SaveToJson();
	//DissolveComponent["material_component"]["material_name"] = mMaterial->name;
	//return DissolveComponent;

	return nlohmann::json();
}

void DissolveComponent::LoadToJson(const nlohmann::json& componentField)
{
	componentField;
	//nlohmann::json DissolveComponentField = componentField["material_component"];

	//mComponentInfo.LoadToJson(DissolveComponentField);

	//std::string materialName = DissolveComponentField["material_name"];
	//SetMaterial(materialName);
}

void DissolveComponent::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Dissolve", true, &mIsUsing))
	{

		Gui::DrawSlider1("Dissolve Frame", dissolve, 0.01f);

		std::string label = mTexture->GetTag();

		const float windowWidth = ImGui::GetContentRegionAvail().x;
		const Vec2 buttonSize = Vec2(windowWidth, 20);
		Gui::DrawButton(label.c_str(), buttonSize);

		// ドロップしたときの処理
		if (Gui::DragDropTarget("DragDrop Texture"))
		{
			std::string tag = MainWindow::GetInstance()->GetDragDropAssetsTag();
			SetTexture(tag);
		}

		if (Gui::BeginTreeNode("Main Texture", true))
		{
			Gui::DrawString("Size : %dx%d",
				(uint32_t)mTexture->GetInitalSize().x, (uint32_t)mTexture->GetInitalSize().y);

			Gui::DrawImage(mTexture, 256);

			Gui::EndTreeNode();
		}
	}

}

void DissolveComponent::SetTexture(const std::string& tag)
{
	ITexture* tex = gAssetsManager->GetTexture(tag);
	if (!tex)
	{
		return;
	}

	mTexture = tex;
}

ITexture* DissolveComponent::GetTexture()
{
	return mTexture;
}
