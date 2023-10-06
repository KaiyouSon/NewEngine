#include "ScissorRectangle.h"
#include "RenderBase.h"
#include "RenderWindow.h"

void ScissorRectangle::Update()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	//------------------------ 繧ｷ繧ｶ繝ｼ遏ｩ蠖｢縺ｮ險ｭ螳壹さ繝槭Φ繝・------------------------//
	// 繧ｷ繧ｶ繝ｼ遏ｩ蠖｢
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // 蛻・ｊ謚懊″蠎ｧ讓吝ｷｦ
	scissorRect.right = scissorRect.left + (LONG)renderWindow->GetWindowSize().x; // 蛻・ｊ謚懊″蠎ｧ讓吝承
	scissorRect.top = 0; // 蛻・ｊ謚懊″蠎ｧ讓吩ｸ・
	scissorRect.bottom = scissorRect.top + (LONG)renderWindow->GetWindowSize().y; // 蛻・ｊ謚懊″蠎ｧ讓吩ｸ・
	// 繧ｷ繧ｶ繝ｼ遏ｩ蠖｢險ｭ螳壹さ繝槭Φ繝峨ｒ縲√さ繝槭Φ繝峨Μ繧ｹ繝医↓遨阪・
	renderBase->GetCommandList()->RSSetScissorRects(1, &scissorRect);
}

