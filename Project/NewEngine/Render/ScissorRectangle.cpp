#include "ScissorRectangle.h"
#include "RenderBase.h"
#include "RenderWindow.h"

void ScissorRectangle::DrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	//D3D12_RECT scissorRect{};
	//scissorRect.left = rect.left; // 蛻・ｊ謚懊″蠎ｧ讓吝ｷｦ
	//scissorRect.right = scissorRect.left + (LONG)renderWindow->GetWindowSize().x; // 蛻・ｊ謚懊″蠎ｧ讓吝承
	//scissorRect.top = rect.top; // 蛻・ｊ謚懊″蠎ｧ讓吩ｸ・
	//scissorRect.bottom = scissorRect.top + (LONG)renderWindow->GetWindowSize().y; // 蛻・ｊ謚懊″蠎ｧ讓吩ｸ・

	// ドローコマンド
	D3D12_RECT scissorRect{};
	scissorRect.left = static_cast<int64_t>(mRect.left);
	scissorRect.right = static_cast<int64_t>(mRect.right);
	scissorRect.top = static_cast<int64_t>(mRect.top);
	scissorRect.bottom = static_cast<int64_t>(mRect.bottom);
	renderBase->GetCommandList()->RSSetScissorRects(1, &scissorRect);
}

void ScissorRectangle::SetRectAngle(const RectAngle rect)
{
	mRect = rect;
}