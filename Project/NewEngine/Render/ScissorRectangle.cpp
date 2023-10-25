#include "ScissorRectangle.h"
#include "RenderBase.h"
#include "RenderWindow.h"

void ScissorRectangle::DrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();

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